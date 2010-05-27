#include "qmpqarchive.h"
#include "qmpqarchive_p.h"

#include "mpqfileinfoiterator.h"

#include <QtGui/QDesktopServices>
#include <QtCore/QFile>
#include <StormLib/StormLib.h>
#include <QDebug>

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
  \fn bool QMPQArchive2::add(const QString & fileName, const QString & archivedName, FileFlags flags, CompressionFlags compression)
  \brief Adds filem specified by \a fileName to archive using \a archivedName.

  Returns true if successful; otherwise returns false.
  While adding file, emits addFileProgressChanged signal
*/
bool QMPQArchive::add(const QString & fileName, const QString & archivedName, FileFlags flags, CompressionFlags compression/*, DWORD dwCompressionNext = 0xFFFFFFFF*/)
{
    SFileSetAddFileCallback(d_func()->mpq, (SFILE_ADDFILE_CALLBACK)addFileCallBack, this);
    bool result = SFileAddFileEx(d_func()->mpq,
                                 fileName.toLocal8Bit().data(),
                                 archivedName.toLocal8Bit().data(),
                                 getAddFileOptionFlags(flags) | MPQ_FILE_REPLACEEXISTING,
                                 getCompressionFlags(compression));
    if (!result) {
        setLastError();
    }
    return result;
}

/*!
  \fn bool QMPQArchive2::add(const QByteArray data, const QString & archivedName, FileFlags flags, CompressionFlags compression)
  \brief Adds \a data to archive specified by \a archivedName.

  Returns true if successful; otherwise returns false.
*/
bool QMPQArchive::add(const QByteArray & data, const QString & archivedName, FileFlags flags, CompressionFlags compression/*, DWORD dwCompressionNext = 0xFFFFFFFF*/)
{
    QString temp = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
    QString fileName = temp + '/' + archivedName;

    QFile file(fileName);
    file.open(QFile::WriteOnly);
    file.write(data);
    file.close();

    return add(fileName, archivedName, flags, compression);
}

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
  \fn bool QMPQArchive2::close()
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
  \fn bool QMPQArchive2::compact()
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
  \fn QString QMPQArchive2::errorString()
  \brief Returns message of last occured error.

*/
QString QMPQArchive::errorString() const
{
    return d_func()->errorString;
}

/*!
  \fn QString QMPQArchive2::errorString(QMPQArchive2::Error code)
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
  \fn MPQFileInfo QMPQArchive2::fileInfo(const QString & fileName)
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
        resultInfo.data->name = QString(buffer);
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
        resultInfo.data->name = QString(buffer);
    }

    return resultInfo;
}

bool QMPQArchive::flush()
{
    if (!SFileFlushArchive(d_func()->mpq)) {
        setLastError();
        return false;
    }
    return true;
}

/*!
  \fn bool QMPQArchive2::exists(const QString & file)
  \brief Returns true if archive contains file \a file

*/
bool QMPQArchive::exists(const QString & file)
{
    if (!checkOpened())
        return false;
    return SFileHasFile(d_func()->mpq, file.toLocal8Bit().data());
}

/*!
  \fn bool QMPQArchive2::isOpened() const
  \brief Returns true if archive is opened

*/
bool QMPQArchive::isOpened() const
{
    return d_func()->isOpened;
}

/*!
  \fn QMPQArchive2::Error QMPQArchive2::lastError()
  \brief Returns error code of last occured error

*/
QMPQArchive::Error QMPQArchive::lastError()
{
    return d_func()->error;
}

/*!
  \fn bool QMPQArchive2::open(const QString & name, OpenFlags flags)
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
  \fn QByteArray QMPQArchive2::read(const QString &file)
  \brief Opens archive specified by \a name with flags \a flags.

  Returns true if successful; otherwise returns false.
*/
QByteArray QMPQArchive::read(const QString &file)
{
    if (!checkOpened())
        return QByteArray();

    void * filePointer = 0;
    bool result;
    result = openFile(file, &filePointer);

    if (!result) {
        setLastError();
        return QByteArray();
    }
    unsigned size = SFileGetFileSize(filePointer, 0);
    char * buffer = new char[size];
    result = SFileReadFile(filePointer, buffer, size, 0, 0);
    if (!result) {
        setLastError();
        return QByteArray();
    }
    QByteArray arr(buffer, size);
    delete [] buffer;
    return arr;
}

/*!
  \fn bool QMPQArchive::remove(const QString & fileName)
  \brief Removes the file \a fileName from currently opened archive.

  Returns true if successful; otherwise returns false.
*/
bool QMPQArchive::remove(const QString & fileName)
{
    if (!checkOpened())
        return false;

    if (!SFileRemoveFile(d_func()->mpq, fileName.toLocal8Bit().data(), 0)) {
        setLastError();
        return false;
    }
    return true;
}

/*!
  \fn bool QMPQArchive2::rename(const QString & oldFileName, const QString & newFileName)
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

QMPQArchive::VerifyArchiveError QMPQArchive::verifyArchive()
{
    if (!checkOpened())
        return NoSignature;

    return (VerifyArchiveError)SFileVerifyArchive(d_func()->mpq);
}

QMPQArchive::VerifyFileError QMPQArchive::verifyFile(const QString & file, Attributes attributes)
{
    if (!checkOpened())
        return ErrorOpenFile;

    return (VerifyFileError)SFileVerifyFile(d_func()->mpq, file.toLocal8Bit().data(), attributes);
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
  \fn quint32 QMPQArchive2::blockTableSize() const
  \brief Returns number of entries in block table

  Returns 0 if no archive opened.
*/
quint32 QMPQArchive::blockTableSize() const
{
    return d_func()->blockTableSize;
}

/*!
  \fn QString QMPQArchive2::file() const
  \brief Returns name of currently opened file

*/
QString QMPQArchive::file() const
{
    return d_func()->file;
}

/*!
  \fn quint32 QMPQArchive2::filesCount() const
  \brief Returns number of files in archive

  Returns 0 if no archive opened.
*/
quint32 QMPQArchive::filesCount() const
{
    return d_func()->filesCount;
}

/*!
  \fn quint32 QMPQArchive2::hashTableSize() const
  \brief Returns size of hash table for currently opened archive.

  Returns 0 if no archive opened.
*/
quint32 QMPQArchive::hashTableSize() const
{
    return d_func()->hashTableSize;
}

/*!
  \fn bool QMPQArchive2::setHashTableSize(quint32 size)
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
  \fn quint32 QMPQArchive2::sectorSize() const
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

quint32 QMPQArchive::getAddFileOptionFlags(FileFlags options)
{
    switch (options) {
    case None : return 0;
    case Implode : return MPQ_FILE_IMPLODE;
    case Compress : return MPQ_FILE_COMPRESS;
    case Compressed : return MPQ_FILE_COMPRESSED;
    case Encrypted : return MPQ_FILE_ENCRYPTED;
    case FixKey : return MPQ_FILE_FIX_KEY;
    case SingleUnit : return MPQ_FILE_SINGLE_UNIT;
    case DeleteMarker : return MPQ_FILE_DELETE_MARKER;
    case SectorCRC : return MPQ_FILE_SECTOR_CRC;
    default: return 0;
    }
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

    quint32 index = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_BLOCKINDEX, &index, sizeof(index));
    data->blockIndex = index;

    quint32 fileSize = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_FILE_SIZE, &fileSize, sizeof(fileSize));
    data->fileSize = fileSize;

    quint32 compressedSize = 0;
    SFileGetFileInfo(hFile, SFILE_INFO_COMPRESSED_SIZE, &compressedSize, sizeof(compressedSize));
    data->compressedSize = compressedSize;

    return resultInfo;
}

QMPQArchive::Error QMPQArchive::lastError(int errorCode)
{
    switch (errorCode) {
    case ERROR_SUCCESS : return NoError;
#ifdef Q_OS_WIN
    case ERROR_SHARING_VIOLATION : return SharingViolation;
#endif
    case ERROR_INVALID_FUNCTION : return InvalidFunction;
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
    case ERROR_HANDLE_DISK_FULL : return HandleDiskFull;
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
        return SFileOpenFileEx(d_func()->mpq, fileName.toLocal8Bit(), 0, hFile);
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


