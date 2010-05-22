#include "qmpqarchive2.h"
#include "qmpqarchive2_p.h"

#include "mpqfileinfoiterator.h"

#include <QtGui/QDesktopServices>
#include <QtCore/QFile>
#include <StormLib/StormLib.h>
#include <QDebug>

QMPQArchive2Private::QMPQArchive2Private():
        isOpened(false), mpq(0), hashTableSize(0)
{
}

QMPQArchive2::QMPQArchive2(QObject *parent) :
    QObject(parent), d_ptr(new QMPQArchive2Private)
{
}

QMPQArchive2::~QMPQArchive2()
{
    closeArchive();
    delete d_ptr;
}

/*!
  \fn bool QMPQArchive2::addFile(const QString & fileName, const QString & archivedName, FileFlags flags, CompressionFlags compression)
  \brief Adds filem specified by \a fileName to archive using \a archivedName.

  Returns true if successful; otherwise returns false.
  While adding file, emits addFileProgressChanged signal
*/
bool QMPQArchive2::addFile(const QString & fileName, const QString & archivedName, FileFlags flags, CompressionFlags compression/*, DWORD dwCompressionNext = 0xFFFFFFFF*/)
{
    SFileSetAddFileCallback(d_func()->mpq, addFileCallBack, this);
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
  \fn bool QMPQArchive2::addFile(const QByteArray data, const QString & archivedName, FileFlags flags, CompressionFlags compression)
  \brief Adds \a data to archive specified by \a archivedName.

  Returns true if successful; otherwise returns false.
*/
bool QMPQArchive2::addFile(const QByteArray & data, const QString & archivedName, FileFlags flags, CompressionFlags compression/*, DWORD dwCompressionNext = 0xFFFFFFFF*/)
{
    QString temp = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
    QString fileName = temp + '/' + archivedName;

    QFile file(fileName);
    file.open(QFile::WriteOnly);
    file.write(data);
    file.close();

    return addFile(fileName, archivedName, flags, compression);
}

MPQFileInfoIterator * QMPQArchive2::beginFileInfoList(const QStringList & listfile, bool includeUnknowns)
{
    return new MPQFileInfoIterator(this, listfile, includeUnknowns);
}

bool QMPQArchive2::createArchive(const QString & name, int hashTableSize, CreateFlags flags)
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

bool QMPQArchive2::createAttributes(Attributes flags)
{
    if (!SFileCreateAttributes(d_func()->mpq, flags)) {
        setLastError();
        return false;
    }
    return true;
}

/*!
  \fn bool QMPQArchive2::closeArchive()
  \brief Closes currently open archive.

  Returns true on success.
*/
bool QMPQArchive2::closeArchive()
{
    Q_D(QMPQArchive2);
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
bool QMPQArchive2::compact()
{
    Q_D(QMPQArchive2);
    SFileSetCompactCallback(d->mpq,
                            reinterpret_cast<SFILE_COMPACT_CALLBACK>(&QMPQArchive2::compactCallBack),
                            this);
    if (!SFileCompactArchive(d->mpq)) {
        setLastError();
        return false;
    }
    return true;
}

/*!
  \fn QString QMPQArchive2::errorString()
  \brief Returns message of last occured error.

*/
QString QMPQArchive2::errorString()
{
    return d_func()->errorString;
}

/*!
  \fn QString QMPQArchive2::errorString(QMPQArchive2::Error code)
  \brief Converts Error \a code to human-readable string.

*/
QString QMPQArchive2::errorString(QMPQArchive2::Error code)
{
    return "";
}

/*!
  \fn MPQFileInfo QMPQArchive2::getFileInfo(const QString & fileName)
  \brief Returns information about \a fileName.

  If archive is not opened or error occured, the result will be invalid.
*/
MPQFileInfo QMPQArchive2::fileInfo(const QString & fileName)
{
    MPQFileInfo resultInfo;

    if (!isOpened())
        return resultInfo;

    void * hFile;
    bool result = openFile(fileName, &hFile);

    if (result)
    {
        resultInfo = getFileInfo_p(hFile);
        // we don't detach data because we're continue filling resultInfo (not changing a copy)
//        qDebug() << "QMPQArchive2::getFileInfo" << resultInfo.data.data()->ref;
        char buffer[255];
        SFileGetFileName(hFile, buffer);
        resultInfo.data->name = QString(buffer);
    }

    return resultInfo;
}

MPQFileInfo QMPQArchive2::fileInfo(quint32 index)
{
    MPQFileInfo resultInfo;

    if (!isOpened())
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

bool QMPQArchive2::flush()
{
    if (!SFileFlushArchive(d_func()->mpq)) {
        setLastError();
        return false;
    }
    return true;
}

/*!
  \fn bool QMPQArchive2::hasFile(const QString & file)
  \brief Returns true if archive contains file \a file

*/
bool QMPQArchive2::hasFile(const QString & file)
{
    return SFileHasFile(d_func()->mpq, file.toLocal8Bit().data());
}

/*!
  \fn bool QMPQArchive2::isOpened() const
  \brief Returns true if archive is opened

*/
bool QMPQArchive2::isOpened() const
{
    return d_func()->isOpened;
}

/*!
  \fn QMPQArchive2::Error QMPQArchive2::lastError()
  \brief Returns error code of last occured error

*/
QMPQArchive2::Error QMPQArchive2::lastError()
{
    return d_func()->error;
}

/*!
  \fn bool QMPQArchive2::openArchive(const QString & name, OpenFlags flags)
  \brief Opens archive specified by \a name with flags \a flags.

  Returns true if successful; otherwise returns false.
*/
bool QMPQArchive2::openArchive(const QString & name, OpenFlags flags)
{
    Q_D(QMPQArchive2);
    if (isOpened()) {
        closeArchive();
    }

    if (!SFileCreateArchiveEx(name.toLocal8Bit().data(), MPQ_OPEN_EXISTING, 0, &d->mpq)) {
        setLastError();
        return false;
    }

    d->file = name;
    d->isOpened = true;
    getArchiveInfo();

    return true;
}

/*!
  \fn QByteArray QMPQArchive2::readFile(const QString &file)
  \brief Opens archive specified by \a name with flags \a flags.

  Returns true if successful; otherwise returns false.
*/
QByteArray QMPQArchive2::readFile(const QString &file)
{
    void * filePointer = 0;
    bool result;
//    result = SFileOpenFileEx(d->mpq, file.toLocal8Bit().data(), 0, &filePointer);
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
  \fn bool QMPQArchive2::renameFile(const QString & oldFileName, const QString & newFileName)
  \brief Removes the file \a fileName from currently opened archive.

  Returns true if successful; otherwise returns false.
*/
bool QMPQArchive2::removeFile(const QString & fileName)
{
    if (!SFileRemoveFile(d_func()->mpq, fileName.toLocal8Bit().data(), 0)) {
        setLastError();
        return false;
    }
    return true;
}

/*!
  \fn bool QMPQArchive2::renameFile(const QString & oldFileName, const QString & newFileName)
  \brief Renames the file \a oldFileName to \a newFileName. Returns true if successful; otherwise returns false.

*/
bool QMPQArchive2::renameFile(const QString & oldFileName, const QString & newFileName)
{
//    if (!isOpened())
//        return false;
    bool result = SFileRenameFile(d_func()->mpq, oldFileName.toLocal8Bit().data(), newFileName.toLocal8Bit().data());
    if (!result) {
        setLastError();
    }
    return result;
}

QMPQArchive2::VerifyArchiveError QMPQArchive2::verifyArchive()
{
    return (VerifyArchiveError)SFileVerifyArchive(d_func()->mpq);
}

bool QMPQArchive2::updateFileAttributes(const QString & fileName)
{
    if (!SFileUpdateFileAttributes(d_func()->mpq, fileName.toLocal8Bit().data())) {
        setLastError();
        return false;
    }
    return true;
}

QMPQArchive2::Attributes QMPQArchive2::attributes() const
{
    return Attributes(SFileGetAttributes(d_func()->mpq));
}

bool QMPQArchive2::setAttributes(Attributes attributes)
{
    return SFileSetAttributes(d_func()->mpq, attributes);
}

/*!
  \fn quint32 QMPQArchive2::blockTableSize() const
  \brief Returns number of entries in block table

  Returns 0 if no archive opened.
*/
quint32 QMPQArchive2::blockTableSize() const
{
    return d_func()->blockTableSize;
}

/*!
  \fn QString QMPQArchive2::file() const
  \brief Returns name of currently opened file

*/
QString QMPQArchive2::file() const
{
    return d_func()->file;
}

/*!
  \fn quint32 QMPQArchive2::filesCount() const
  \brief Returns number of files in archive

  Returns 0 if no archive opened.
*/
quint32 QMPQArchive2::filesCount() const
{
    return d_func()->filesCount;
}

/*!
  \fn quint32 QMPQArchive2::hashTableSize() const
  \brief Returns size of hash table for currently opened archive.

  Returns 0 if no archive opened.
*/
quint32 QMPQArchive2::hashTableSize() const
{
    return d_func()->hashTableSize;
}

/*!
  \fn bool QMPQArchive2::setHashTableSize(quint32 size)
  \brief Changes the size of hash table for currently opened archive to \a size.

  Returns true if successful; otherwise returns false.
*/
bool QMPQArchive2::setHashTableSize(quint32 size)
{
    if (!isOpened())
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
quint32 QMPQArchive2::sectorSize() const
{
    return d_func()->sectorSize;
}

// ============== private ==============

//QMPQArchive2::Error QMPQArchive2::GetLastError()
//{
//    return lastError(GetLastError());
//}

void QMPQArchive2::addFileCallBack(void *o, quint32 bytesWritten, quint32 totalBytes, bool finalCall)
{
    QMPQArchive2 * q = reinterpret_cast<QMPQArchive2 *>(o);
    emit q->addFileProgressChanged(bytesWritten, totalBytes, finalCall);
}

void QMPQArchive2::compactCallBack(void * o , int op, qint64 * bytesProcessed, qint64 * bytesTotal)
{
    QMPQArchive2 * q = reinterpret_cast<QMPQArchive2 *>(o);
    emit q->compactProgressChanged((QMPQArchive2::CompactOperation)op, *bytesProcessed, *bytesTotal);
}

void QMPQArchive2::getArchiveInfo()
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

quint32 QMPQArchive2::getCompressionFlags(CompressionFlags types)
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

quint32 QMPQArchive2::getAddFileOptionFlags(FileFlags options)
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

MPQFileInfo QMPQArchive2::getFileInfo_p(void * hFile)
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

QMPQArchive2::Error QMPQArchive2::lastError(int errorCode)
{
    return NoError;
}

bool QMPQArchive2::openFile(const QString & fileName, void ** hFile)
{
    if (fileName.startsWith("File")) {
        quint32 index = fileName.mid(5, 13).toInt(); // we get block index from file name
        return SFileOpenFileEx(d_func()->mpq, (const char *)index, SFILE_OPEN_BY_INDEX, hFile);
    } else {
        return SFileOpenFileEx(d_func()->mpq, fileName.toLocal8Bit(), 0, hFile);
    }
}

void QMPQArchive2::setLastError()
{
    d_func()->error = lastError(GetLastError());
    d_func()->errorString = errorString(d_func()->error);
    emit error();
}

