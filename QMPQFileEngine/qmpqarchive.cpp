#include "qmpqarchive.h"
#include "qmpqarchive_p.h"

#include "mpqfileinfo.h"
#include "mpqfileinfoiterator.h"

#include <QtCore/QFile>
#include <StormLib/StormLib.h>
#include <QDebug>

QLocale QMPQArchivePrivate::staticLocale = QLocale(QLocale::C);

QMPQArchivePrivate::QMPQArchivePrivate():
        isOpened(false), mpq(0), hashTableSize(0)
{
}

QMPQArchive::QMPQArchive(QObject *parent) :
    QObject(parent), d_ptr(new QMPQArchivePrivate)
{
}

QMPQArchive::~QMPQArchive()
{
    close();
    delete d_ptr;
}

/*!
  \fn bool QMPQArchive::add(const QString & fileName, const QString & archivedName, FileFlags flags, CompressionFlags compression)
  \brief Adds filem specified by \a fileName to archive using \a archivedName.

  Returns true if successful; otherwise returns false.
  While adding file, emits addFileProgressChanged signal
*/
bool QMPQArchive::add(const QString & fileName, const QString & archivedName, FileFlags flags, CompressionFlags compression/*, DWORD dwCompressionNext = 0xFFFFFFFF*/)
{
    Q_D(QMPQArchive);
    SFileSetAddFileCallback(d->mpq, (SFILE_ADDFILE_CALLBACK)addFileCallBack, this);
    bool result = SFileAddFileEx(d_func()->mpq,
                                 fileName.toLocal8Bit().data(),
                                 archivedName.toLocal8Bit().data(),
                                 getFileFlags(flags) | MPQ_FILE_REPLACEEXISTING,
                                 getCompressionFlags(compression));
    if (!result) {
        setLastError();
    }
    d->filesCount++;
    return result;
}

/*!
  \fn bool QMPQArchive::add(const QByteArray data, const QString & archivedName, FileFlags flags, CompressionFlags compression)
  \brief Adds \a data to archive specified by \a archivedName.

  Returns true if successful; otherwise returns false.
*/
bool QMPQArchive::add(const QByteArray & data, const QString & archivedName, FileFlags flags, CompressionFlags compression/*, DWORD dwCompressionNext = 0xFFFFFFFF*/)
{
    Q_D(QMPQArchive);
    void * hFile;
    bool result = true;
    int size = data.size();

    if (!checkOpened())
        return false;

    result &= SFileCreateFile(d->mpq,
                             archivedName.toLocal8Bit().data(),
                             0,
                             size,
                             0,
                             getFileFlags(flags) | MPQ_FILE_REPLACEEXISTING,
                             &hFile);
    if (!result) {
        setLastError();
        return result;
    }

    result &= SFileWriteFile(hFile, (void*)(data.data()), size, getCompressionFlags(compression));
    if (!result) {
        setLastError();
        // we have to free resources so we continue operation
    }

    result &= SFileFinishFile(hFile);
    if (!result) {
        setLastError();
        return result;
    }
    d->filesCount++;
    return result;
}

/*!
  \fn MPQFileInfoIterator * QMPQArchive::beginFileInfoList(const QStringList & listfile, bool includeUnknowns)
  \brief Return class to iterate over files in arcvhive.

  NOTE: this object must be deleted manually after being used.
*/
MPQFileInfoIterator * QMPQArchive::beginFileInfoList(const QStringList & listfile, bool includeUnknowns)
{
    return new MPQFileInfoIterator(this, listfile, includeUnknowns);
}

bool QMPQArchive::create(const QString & name, int hashTableSize, CreateFlags flags)
{
    void * mpq = 0;
    if (!SFileCreateArchiveEx(name.toLocal8Bit().data(), MPQ_CREATE_NEW | flags, hashTableSize, &mpq)) {
        setLastError();
        return false;
    }
    SFileCloseArchive(mpq);
    mpq = 0;

    return true;
}

bool QMPQArchive::createAttributes(Attributes flags)
{
    if (!checkOpened())
        return false;

    if (!SFileCreateAttributes(d_func()->mpq, flags)) {
        setLastError();
        return false;
    }
    return true;
}

/*!
  \fn bool QMPQArchive::close()
  \brief Closes currently open archive.

  Returns true on success.
*/
bool QMPQArchive::close()
{
    Q_D(QMPQArchive);
    if (!isOpened()) {
        return true;
    }
    bool result = SFileCloseArchive(d->mpq);
    if (!result) {
        setLastError();
        return false;
    }
    d->mpq = 0;
    d->isOpened = false;
    d->file = "";
    d->hashTableSize = 0;
    return true;
}

/*!
  \fn bool QMPQArchive::compact()
  \brief Compacts currently open archive.

  Returns true on success.

  This function is used for optimizing archive.

  While compacting, emits compactProgressChanged signal
*/
bool QMPQArchive::compact()
{
    Q_D(QMPQArchive);
    checkOpened();
    SFileSetCompactCallback(d->mpq,
                            reinterpret_cast<SFILE_COMPACT_CALLBACK>(&QMPQArchive::compactCallBack),
                            this);
    if (!SFileCompactArchive(d->mpq)) {
        setLastError();
        return false;
    }
    return true;
}

/*!
  \fn const QList<MPQFileInfo> QMPQArchive::entryList(const QStringList & listfile, bool includeUnknowns)
  \brief Returns list of filsinfos for every file in archive.

  \a listfile is used with internal listfile.
  If \a includeUnknowns is true, also includes entries for unknown files.
*/
const QList<MPQFileInfo> QMPQArchive::entryList(const QStringList & listfile, bool includeUnknowns)
{
    QList<MPQFileInfo> result;
    MPQFileInfoIterator * iterator = beginFileInfoList(listfile, includeUnknowns);
    while (iterator->hasNext()) {
        result.append(iterator->next());
    }
    delete iterator;
    return result;
}

/*!
  \fn QString QMPQArchive::errorString()
  \brief Returns message of last occured error.

*/
QString QMPQArchive::errorString() const
{
    return d_func()->errorString;
}

/*!
  \fn QString QMPQArchive::errorString(QMPQArchive::Error code)
  \brief Converts Error \a code to human-readable string.

*/
QString QMPQArchive::errorString(QMPQArchive::Error code) const
{
    switch (code) {
    case NoError:
        return "";
#ifdef Q_OS_WIN
    case SharingViolation :
        return tr("sharing violation");
#endif
    case ArchiveNotOpened:
        return tr("archive not opened");
    case InvalidFunction:
        return tr("function not implemented");
    case FileNotFound:
        return tr("file not found");
    case AccessDenied:
        return tr("access denied");
    case InvalidHandle:
        return tr("invalid handle");
    case NotEnoughMemory:
        return tr("not enough memory");
    case BadFormat:
        return tr("bad format");
    case NoMoreFiles:
        return tr("no more files");
    case WriteFault:
        return tr("unable to write to device");
    case HandleEndOfFile:
        return tr("access beyond end of file");
    case HandleDiskFull:
        return tr("no space left on device");
    case InvalidParameter:
        return tr("invalid parameter");
    case DiskFull:
        return tr("no space left on device");
    case AlreadyExists:
        return tr("file already exists");
    case CanNotComplete:
        return tr("operation cannot be completed");
    case InsufficientBuffer:
        return tr("insufficient buffer");
    case UnknownError:
    default:
        return tr("unknown error");
    }
}

/*!
  \fn MPQFileInfo QMPQArchive::fileInfo(const QString & fileName)
  \brief Returns information about \a fileName.

  If archive is not opened or error occured, the result will be invalid.
*/
MPQFileInfo QMPQArchive::fileInfo(const QString & fileName)
{
    MPQFileInfo resultInfo;

    if (!checkOpened())
        return resultInfo;

    void * hFile;
    bool result = openFile(fileName, &hFile);

    if (result)
    {
        resultInfo = getFileInfo_p(hFile);
        // we don't detach data because we're continue filling resultInfo (not changing a copy)
        char buffer[255];
        SFileGetFileName(hFile, buffer);
        resultInfo.data->setName(QString(buffer));
        SFileCloseFile(hFile);
    }

    return resultInfo;
}

MPQFileInfo QMPQArchive::fileInfo(quint32 index)
{
    MPQFileInfo resultInfo;

    if (!checkOpened())
        return resultInfo;

    void * hFile;
    bool result = SFileOpenFileEx(d_func()->mpq, (const char *)index, SFILE_OPEN_BY_INDEX, &hFile);
    if (result)
    {
        resultInfo = getFileInfo_p(hFile);
        // we don't detach data because we're continue filling resultInfo (not changing a copy)
        char buffer[255];
        SFileGetFileName(hFile, buffer);
        resultInfo.data->setName(QString(buffer));
        SFileCloseFile(hFile);
    }

    return resultInfo;
}

/*!
  \fn bool QMPQArchive::flush()
  \brief Flushes currently opened archive

*/
bool QMPQArchive::flush()
{
    if (!SFileFlushArchive(d_func()->mpq)) {
        setLastError();
        return false;
    }
    return true;
}

/*!
  \fn bool QMPQArchive::exists(const QString & file)
  \brief Returns true if archive contains file \a file

*/
bool QMPQArchive::exists(const QString & file)
{
    if (!checkOpened())
        return false;
    return SFileHasFile(d_func()->mpq, file.toLocal8Bit().data());
}

/*!
  \fn bool QMPQArchive::isOpened() const
  \brief Returns true if archive is opened

*/
bool QMPQArchive::isOpened() const
{
    return d_func()->isOpened;
}

/*!
  \fn QMPQArchive::Error QMPQArchive::lastError()
  \brief Returns error code of last occured error

*/
QMPQArchive::Error QMPQArchive::lastError()
{
    return d_func()->error;
}

/*!
  \fn bool QMPQArchive::open(const QString & name, OpenFlags flags)
  \brief Opens archive specified by \a name with flags \a flags.

  Returns true if successful; otherwise returns false.
*/
bool QMPQArchive::open(const QString & name, OpenFlags flags)
{
    Q_D(QMPQArchive);
    if (isOpened()) {
        close();
    }

    bool result = SFileCreateArchiveEx(name.toLocal8Bit().data(),
                                      getOpenFlags(flags) | MPQ_OPEN_EXISTING,
                                      0,
                                      &d->mpq);
    if (!result) {
        setLastError();
        return false;
    }

    d->file = name;
    d->isOpened = true;
    getArchiveInfo();

    return true;
}

/*!
  \fn QByteArray QMPQArchive::read(const QString &file)
  \brief Opens archive specified by \a name with flags \a flags.

  Returns true if successful; otherwise returns false.
*/
QByteArray QMPQArchive::read(const QString &file)
{
    if (!checkOpened())
        return QByteArray();

    void * hFile = 0;
    bool ok;
    ok = openFile(file, &hFile);

    if (!ok) {
        setLastError();
        return QByteArray();
    }

    unsigned size = SFileGetFileSize(hFile, 0);
    char * buffer = new char[size];
    ok = SFileReadFile(hFile, buffer, size, 0, 0);
    QByteArray result;
    if (!ok) {
        setLastError();
    } else {
        result = QByteArray(buffer, size);
    }
    SFileCloseFile(hFile);
    delete [] buffer;
    return result;
}

/*!
  \fn bool QMPQArchive::remove(const QString & fileName)
  \brief Removes the file \a fileName from currently opened archive.

  Returns true if successful; otherwise returns false.
*/
bool QMPQArchive::remove(const QString & fileName)
{
    Q_D(QMPQArchive);
    if (!checkOpened())
        return false;

    if (!SFileRemoveFile(d->mpq, fileName.toLocal8Bit().data(), 0)) {
        setLastError();
        return false;
    }
    d->filesCount--;
    return true;
}

/*!
  \fn bool QMPQArchive::rename(const QString & oldFileName, const QString & newFileName)
  \brief Renames the file \a oldFileName to \a newFileName. Returns true if successful; otherwise returns false.

*/
bool QMPQArchive::rename(const QString & oldFileName, const QString & newFileName)
{
    if (!checkOpened())
        return false;

    bool result = SFileRenameFile(d_func()->mpq, oldFileName.toLocal8Bit().data(), newFileName.toLocal8Bit().data());
    if (!result) {
        setLastError();
    }
    return result;
}

bool QMPQArchive::setFileLocale(const QString & name, const QLocale & locale)
{
    int localeId = getLocaleId(locale);
    void * hFile = 0;
    bool result;
    result = openFile(name, &hFile);
    if (!result) {
        setLastError();
        return result;
    }
    result = SFileSetFileLocale(hFile, localeId);
    if (!result) {
        setLastError();
//        return result;
    }
    SFileCloseFile(hFile);
    return result;
}

QLocale QMPQArchive::locale()
{
//    Q_D(QMPQArchive);
//    return d->locale;
    return QMPQArchivePrivate::staticLocale;
}

void QMPQArchive::setLocale(const QLocale & locale)
{
//    Q_D(QMPQArchive);
//    d->locale = locale;
    QMPQArchivePrivate::staticLocale = locale;
    int localeId = getLocaleId(locale);
    SFileSetLocale(localeId);
}

QMPQArchive::VerifyArchiveError QMPQArchive::verifyArchive()
{
    if (!checkOpened())
        return NoSignature;

    return (VerifyArchiveError)SFileVerifyArchive(d_func()->mpq);
}

QMPQArchive::VerifyFileFlags QMPQArchive::verifyFile(const QString & file, Attributes attributes)
{
    if (!checkOpened())
        return ErrorOpenFile;

    return (VerifyFileFlag)SFileVerifyFile(d_func()->mpq, file.toLocal8Bit().data(), attributes);
}

bool QMPQArchive::updateFileAttributes(const QString & fileName)
{
    if (!checkOpened())
        return false;

    if (!SFileUpdateFileAttributes(d_func()->mpq, fileName.toLocal8Bit().data())) {
        setLastError();
        return false;
    }
    return true;
}

QMPQArchive::Attributes QMPQArchive::attributes() const
{
    return Attributes(SFileGetAttributes(d_func()->mpq));
}

bool QMPQArchive::setAttributes(Attributes attributes)
{
    if (!checkOpened())
        return false;

    return SFileSetAttributes(d_func()->mpq, attributes);
}

/*!
  \fn quint32 QMPQArchive::blockTableSize() const
  \brief Returns number of entries in block table

  Returns 0 if no archive opened.
*/
quint32 QMPQArchive::blockTableSize() const
{
    return d_func()->blockTableSize;
}

/*!
  \fn QString QMPQArchive::file() const
  \brief Returns name of currently opened file

*/
QString QMPQArchive::file() const
{
    return d_func()->file;
}

/*!
  \fn quint32 QMPQArchive::filesCount() const
  \brief Returns number of files in archive

  Returns 0 if no archive opened.
*/
quint32 QMPQArchive::filesCount() const
{    
    return d_func()->filesCount;
}

/*!
  \fn quint32 QMPQArchive::hashTableSize() const
  \brief Returns size of hash table for currently opened archive.

  Returns 0 if no archive opened.
*/
quint32 QMPQArchive::hashTableSize() const
{
    return d_func()->hashTableSize;
}

/*!
  \fn bool QMPQArchive::setHashTableSize(quint32 size)
  \brief Changes the size of hash table for currently opened archive to \a size.

  Returns true if successful; otherwise returns false.
*/
bool QMPQArchive::setHashTableSize(quint32 size)
{
    if (!checkOpened())
        return false;

    bool result = SFileSetHashTableSize(d_func()->mpq, size);
    if (!result) {
        setLastError();
    }
    return result;
}

/*!
  \fn quint32 QMPQArchive::sectorSize() const
  \brief Returns size of file sector in bytes.

  Returns 0 if no archive opened.
*/
quint32 QMPQArchive::sectorSize() const
{
    return d_func()->sectorSize;
}

// ============== private ==============

void QMPQArchive::addFileCallBack(void *o, quint32 bytesWritten, quint32 totalBytes, bool finalCall)
{
    QMPQArchive * q = reinterpret_cast<QMPQArchive *>(o);
    emit q->addFileProgressChanged(bytesWritten, totalBytes, finalCall);
}

void QMPQArchive::compactCallBack(void * o , int op, qint64 * bytesProcessed, qint64 * bytesTotal)
{
    QMPQArchive * q = reinterpret_cast<QMPQArchive *>(o);
    emit q->compactProgressChanged((QMPQArchive::CompactOperation)op, *bytesProcessed, *bytesTotal);
}

bool QMPQArchive::checkOpened()
{
    unsetError();
    if (!isOpened()) {
        d_func()->error = ArchiveNotOpened;
        d_func()->errorString = errorString(ArchiveNotOpened);
        return false;
    }
    return true;
}

void QMPQArchive::getArchiveInfo()
{
    if (isOpened()) {
        quint32 result = 0;
        if (!SFileGetFileInfo(d_func()->mpq, SFILE_INFO_ARCHIVE_SIZE, &result, sizeof(result))) {
            setLastError();
        }
        d_func()->archiveSize = result;

        if (!SFileGetFileInfo(d_func()->mpq, SFILE_INFO_HASH_TABLE_SIZE, &result, sizeof(result))) {
            setLastError();
        }
        d_func()->hashTableSize = result;

        if (!SFileGetFileInfo(d_func()->mpq, SFILE_INFO_BLOCK_TABLE_SIZE, &result, sizeof(result))) {
            setLastError();
        }
        d_func()->blockTableSize = result;


        if (!SFileGetFileInfo(d_func()->mpq, SFILE_INFO_SECTOR_SIZE, &result, sizeof(result))) {
            setLastError();
        }
        d_func()->sectorSize = result;

        if (!SFileGetFileInfo(d_func()->mpq, SFILE_INFO_NUM_FILES, &result, sizeof(result))) {
            setLastError();
        }
        d_func()->filesCount = result;

    }
}

quint32 QMPQArchive::getFileFlags(FileFlags flags)
{
    quint32 result = 0;
    if (flags & Implode)
        result |= MPQ_FILE_IMPLODE;
    if (flags & Compress)
        result |= MPQ_FILE_COMPRESS;
    if (flags & Compressed)
        result |= MPQ_FILE_COMPRESSED;
    if (flags & Encrypted)
        result |= MPQ_FILE_ENCRYPTED;
    if (flags & FixKey)
        result |= MPQ_FILE_FIX_KEY;
    if (flags & SingleUnit)
        result |= MPQ_FILE_SINGLE_UNIT;
    if (flags & DeleteMarker)
        result |= MPQ_FILE_DELETE_MARKER;
    if (flags & SectorCRC)
        result |= MPQ_FILE_SECTOR_CRC;
    return result;
}

QMPQArchive::FileFlags QMPQArchive::getFileFlags(quint32 flags)
{
    QMPQArchive::FileFlags result;
    if (flags & MPQ_FILE_IMPLODE)
        result |= Implode;
    if (flags & MPQ_FILE_COMPRESS)
        result |= Compress;
    if (flags & MPQ_FILE_COMPRESSED)
        result |= Compressed;
    if (flags & MPQ_FILE_ENCRYPTED)
        result |= Encrypted;
    if (flags & MPQ_FILE_FIX_KEY)
        result |= FixKey;
    if (flags & MPQ_FILE_SINGLE_UNIT)
        result |= SingleUnit;
    if (flags & MPQ_FILE_DELETE_MARKER)
        result |= DeleteMarker;
    if (flags & MPQ_FILE_SECTOR_CRC)
        result |= SectorCRC;
    return result;
}

quint32 QMPQArchive::getCompressionFlags(CompressionFlags types)
{
    switch (types) {
    case NoCompression : return 0;
    case HUFFMAN : return MPQ_COMPRESSION_HUFFMANN;
    case ZLIB : return MPQ_COMPRESSION_ZLIB;
    case PKWARE : return MPQ_COMPRESSION_PKWARE;
    case BZIP2 : return MPQ_COMPRESSION_BZIP2;
    case ADCPM_Mono : return MPQ_COMPRESSION_WAVE_MONO;
    case ADCPM_Stereo : return MPQ_COMPRESSION_WAVE_STEREO;
    default: return 0;
    }
}

QLocale QMPQArchive::getLocale(int localeId)
{
    switch (localeId) {
    case 0x0407: return QLocale(QLocale::German, QLocale::Germany);
    case 0x0809: return QLocale(QLocale::English, QLocale::UnitedKingdom);
    case 0x0409: return QLocale(QLocale::English, QLocale::UnitedStates);
    case 0x040A: return QLocale(QLocale::Spanish, QLocale::Spain);
    case 0x040C: return QLocale(QLocale::French, QLocale::France);
    case 0x0410: return QLocale(QLocale::Italian, QLocale::Italy);
    case 0x0412: return QLocale(QLocale::Korean, QLocale::DemocraticRepublicOfKorea);
    case 0x0415: return QLocale(QLocale::Polish, QLocale::Poland);
    case 0x0419: return QLocale(QLocale::Russian, QLocale::RussianFederation);
    case 0x0404: return QLocale(QLocale::Chinese, QLocale::China);
    default: return QLocale(QLocale::C);
    }
}

int QMPQArchive::getLocaleId(const QLocale & locale)
{
    if (locale == QLocale(QLocale::German, QLocale::Germany))
        return 0x0407;
    if (locale == QLocale(QLocale::English, QLocale::UnitedKingdom))
        return 0x0809;
    if (locale == QLocale(QLocale::English, QLocale::UnitedStates))
        return 0x0409;
    if (locale == QLocale(QLocale::Spanish, QLocale::Spain))
        return 0x040A;
    if (locale == QLocale(QLocale::French, QLocale::France))
        return 0x040C;
    if (locale == QLocale(QLocale::Italian, QLocale::Italy))
        return 0x0410;
    if (locale == QLocale(QLocale::Korean, QLocale::DemocraticRepublicOfKorea))
        return 0x0412;
    if (locale == QLocale(QLocale::Polish, QLocale::Poland))
        return 0x0415;
    if (locale == QLocale(QLocale::Russian, QLocale::RussianFederation))
        return 0x0419;
    if (locale == QLocale(QLocale::Chinese, QLocale::China))
        return 0x0404;
    if (locale == QLocale(QLocale::C))
        return 0;
    return 0;
}

quint32 QMPQArchive::getOpenFlags(OpenFlags flags)
{
    switch (flags) {
    case NoCompression : return 0;
    case NoListfile : return MPQ_OPEN_NO_LISTFILE;
    case NoAttributes : return MPQ_OPEN_NO_ATTRIBUTES;
    case ForceMPQ1 : return MPQ_OPEN_FORCE_MPQ_V1;
    case CheckSectorCRC : return MPQ_OPEN_CHECK_SECTOR_CRC;
    default: return 0;
    }
}

MPQFileInfo QMPQArchive::getFileInfo_p(void * hFile)
{
    MPQFileInfo resultInfo;
    TMPQFile * hf;
    hf = (TMPQFile *)hFile;

    resultInfo.data.detach(); // we explicitly detach data to be sure that we don't break anything
    MPQFileInfoData * data = resultInfo.data.data();

    data->valid = true;

    quint32 hashIndex = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_HASH_INDEX, &hashIndex, sizeof(hashIndex));
    data->hashIndex = hashIndex;

    quint32 codename1 = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_CODENAME1, &codename1, sizeof(codename1));
    data->codename1 = codename1;

    quint32 codename2 = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_CODENAME2, &codename2, sizeof(codename2));
    data->codename1 = codename2;

    quint32 localeId = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_LOCALEID, &localeId, sizeof(localeId));
//    data->localeId = localeId;
    data->locale = getLocale(localeId);

    quint32 index = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_BLOCKINDEX, &index, sizeof(index));
    data->blockIndex = index;

    quint32 fileSize = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_FILE_SIZE, &fileSize, sizeof(fileSize));
    data->fileSize = fileSize;

    quint32 compressedSize = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_COMPRESSED_SIZE, &compressedSize, sizeof(compressedSize));
    data->compressedSize = compressedSize;

    quint32 fileFlags = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_FLAGS, &fileFlags, sizeof(fileFlags));
    data->fileFlags = getFileFlags(fileFlags);

    quint32 fileTime = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_FILETIME, &fileTime, sizeof(fileTime));
    data->fileTime = fileTime;

    return resultInfo;
}

QMPQArchive::Error QMPQArchive::lastError(int errorCode)
{
    switch (errorCode) {
    case ERROR_SUCCESS : return NoError;
#ifdef Q_OS_WIN
    case ERROR_SHARING_VIOLATION : return SharingViolation;
#endif
//    case ERROR_INVALID_FUNCTION : return InvalidFunction;
    case ERROR_FILE_NOT_FOUND : return FileNotFound;
    case ERROR_ACCESS_DENIED : return AccessDenied;
    case ERROR_INVALID_HANDLE : return InvalidHandle;
    case ERROR_NOT_ENOUGH_MEMORY : return NotEnoughMemory;
    case ERROR_BAD_FORMAT : return BadFormat;
    case ERROR_NO_MORE_FILES : return NoMoreFiles;
    case ERROR_WRITE_FAULT : return WriteFault;
//    case ERROR_READ_FAULT : return ReadFault;
    case ERROR_GEN_FAILURE : return GenerateFailure;
    case ERROR_HANDLE_EOF : return HandleEndOfFile;
//    case ERROR_HANDLE_DISK_FULL : return HandleDiskFull;
//    case ERROR_NOT_SUPPORTED : return NotSupported;
    case ERROR_INVALID_PARAMETER : return InvalidParameter;
    case ERROR_DISK_FULL : return DiskFull;
//    case ERROR_CALL_NOT_IMPLEMENTED : return CallNotImplemented;
    case ERROR_ALREADY_EXISTS : return AlreadyExists;
    case ERROR_CAN_NOT_COMPLETE : return CanNotComplete;
//    case ERROR_PARAMETER_QUOTA_EXCEEDED : return ParameterQuotaExceeded;
//    case ERROR_FILE_CORRUPT : return FileCorrupt;
    case ERROR_INSUFFICIENT_BUFFER : return InsufficientBuffer;
    }
    return UnknownError;
}

bool QMPQArchive::openFile(const QString & fileName, void ** hFile)
{
    if (fileName.startsWith("File")) {
        quint32 index = fileName.mid(4, 8).toInt(); // we get block index from file name
        return SFileOpenFileEx(d_func()->mpq, (const char *)index, SFILE_OPEN_BY_INDEX, hFile);
    } else {
        int localeId = getLocaleId(locale());
        SFileSetLocale(localeId);
        bool result = SFileOpenFileEx(d_func()->mpq, fileName.toLocal8Bit(), 0, hFile);
        SFileSetLocale(0);
        return result;
    }
}

void QMPQArchive::setLastError()
{
    int errorCode = GetLastError();
    d_func()->error = lastError(errorCode);
    d_func()->errorString = errorString(d_func()->error);
    qWarning() << "Mpq Error: " << d_func()->errorString;
    emit error();
}

void QMPQArchive::unsetError()
{
    d_func()->error = NoError;
    d_func()->errorString = "";
}


