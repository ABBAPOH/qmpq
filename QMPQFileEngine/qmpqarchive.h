#ifndef QMPQARCHIVE2_H
#define QMPQARCHIVE2_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "QMPQFileEngine_global.h"
//#include "mpqfileinfo.h"
//#include "mpqfileinfoiterator.h"

class QMPQArchivePrivate;
class MPQFileInfo;
class MPQFileInfoIterator;
class QMPQFILEENGINESHARED_EXPORT QMPQArchive : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMPQArchive)
    Q_PROPERTY(QString file READ file)
    Q_PROPERTY(bool isOpened READ isOpened)
    Q_PROPERTY(quint32 blockTableSize READ hashTableSize)
    Q_PROPERTY(quint32 filesCount READ filesCount)
    Q_PROPERTY(quint32 hashTableSize READ hashTableSize WRITE setHashTableSize)
    Q_PROPERTY(quint32 sectorSize READ sectorSize)
    Q_PROPERTY(Attributes attributes READ attributes WRITE setAttributes)

    friend class MPQFileInfoIterator;

public:
    enum Error
    {
        NoError = 0,
#ifdef Q_OS_WIN
        SharingViolation,
#endif
        ArchiveNotOpened,
        InvalidFunction,
        FileNotFound,
        AccessDenied,
        InvalidHandle,
        NotEnoughMemory,
        BadFormat,
        NoMoreFiles,
        WriteFault,
//        ReadFault,
        GenerateFailure,
        HandleEndOfFile,
        HandleDiskFull,
//        NotSupported,
        InvalidParameter,
        DiskFull,
//        CallNotImplemented,
        AlreadyExists,
        CanNotComplete,
//        ParameterQuotaExceeded,
//        FileCorrupt,
        InsufficientBuffer,
        UnknownError
    };
    Q_ENUMS(Error);

    enum OpenFlag
    {
        OpenFlagNone = 0,
        NoListfile = 0x01,
        NoAttributes = 0x02,
        ForceMPQ1 = 0x04,
        CheckSectorCRC = 0x08
    };
    Q_DECLARE_FLAGS(OpenFlags, OpenFlag);

    enum CreateFlag
    {
        ArchiveV1 = 0x00000000,
        ArchiveV2 = 0x00010000,
        AddAttributes = 0x00100000
    };
    Q_DECLARE_FLAGS(CreateFlags, CreateFlag);

    enum CompactOperation {
        CheckingFiles = 1,
        CheckingHashTableSize,
        CopyingNonMPQData,
        CompactingFiles,
        ClosingArchive
    };
    Q_ENUMS(CompactOperation);

    enum FileFlag {
        None = 0x00,
        Implode = 0x01,
        Compress = 0x02,
        Compressed = 0x04,
        Encrypted = 0x08,
        FixKey = 0x10,
        SingleUnit = 0x20,
        DeleteMarker = 0x40,
        SectorCRC = 0x80
    };
    Q_DECLARE_FLAGS(FileFlags, FileFlag)
    Q_FLAGS(FileFlags)

    enum CompressionFlag {
        NoCompression = 0x00,
        HUFFMAN = 0x01,
        PKWARE = 0x02,
        ZLIB = 0x04,
        BZIP2 = 0x08,
        LZMA = 0x10,
        SPARSE = 0x20,
        ADCPM_Mono = 0x40,
        ADCPM_Stereo = 0x80
    };
    Q_DECLARE_FLAGS(CompressionFlags, CompressionFlag)
    Q_FLAGS(CompressionFlags)

    enum VerifyArchiveError {
        NoSignature = 0,
        VerifyFailed,
        WeakSignatureOk,
        WeakSignatureError,
        StrongSignatureOk,
        StrongSignatureError
    };
    Q_ENUMS(VerifyArchiveError);

    enum VerifyFileFlag {
        VerifyFileOk = 0x00,
        ErrorOpenFile = 0x01,
        ErrorReadFile = 0x02,
        ErrorSectorChecksum = 0x04,
        ErrorFileChecksum = 0x08,
        ErrorMD5 = 0x10
    };
    Q_DECLARE_FLAGS(VerifyFileFlags, VerifyFileFlag)
    Q_FLAGS(VerifyFileFlags);

    enum Attribute {
        AttributesNone = 0,
        CRC32 = 1,
        FileTime = 2,
        MD5 = 4
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)
    Q_FLAGS(Attributes)

    explicit QMPQArchive(QObject *parent = 0);
    ~QMPQArchive();

    bool add(const QString & file, const QString & archivedName, FileFlags flags = None, CompressionFlags compression = NoCompression/*, DWORD dwCompressionNext = 0xFFFFFFFF*/);
    bool add(const QByteArray & data, const QString & archivedName, FileFlags flags = None, CompressionFlags compression = NoCompression/*, DWORD dwCompressionNext = 0xFFFFFFFF*/);
    MPQFileInfoIterator * beginFileInfoList(const QStringList & listfile = QStringList(), bool includeUnknowns = true);
    bool create(const QString & name, int hashTableSize, CreateFlags flags = ArchiveV1);
    bool createAttributes(Attributes);
    bool close();
    bool compact();
    const QList<MPQFileInfo> entryList(const QStringList & listfile = QStringList(), bool includeUnknowns = true);
    QString errorString() const;
    QString errorString(Error code) const;
    MPQFileInfo fileInfo(const QString & file);
    MPQFileInfo fileInfo(quint32 index);
    bool flush();
    bool exists(const QString & file);
    Error lastError();
    bool open(const QString & name, OpenFlags flags = OpenFlagNone);
    QByteArray read(const QString &file);
    bool remove(const QString & fileName);
    bool rename(const QString & oldFileName, const QString & newFileName);
    VerifyArchiveError verifyArchive();
    VerifyFileFlags verifyFile(const QString & file, Attributes attributes);
    bool updateFileAttributes(const QString & fileName);

    Attributes attributes() const;
    bool setAttributes(Attributes attributes);

    quint32 blockTableSize() const;

    QString file() const;

    quint32 filesCount() const;

    quint32 hashTableSize() const;
    bool setHashTableSize(quint32 size);

    bool isOpened() const;

    quint32 sectorSize() const;

signals:
    void error();
    void compactProgressChanged(QMPQArchive::CompactOperation op, qint64 bytesProcessed, qint64 bytesTotal);
    void addFileProgressChanged(quint32 bytesWritten, quint32 totalBytes, bool finalCall);

public slots:

protected:
    QMPQArchivePrivate * d_ptr;

private:
    static void addFileCallBack(void *, quint32 bytesWritten, quint32 totalBytes, bool finalCall);
    static void compactCallBack(void *, int, qint64 * bytesProcessed, qint64 * bytesTotal);
    bool checkOpened();
    void getArchiveInfo();
    quint32 getFileFlags(FileFlags flags);
    FileFlags getFileFlags(quint32 flags);
    quint32 getCompressionFlags(CompressionFlags types);
    quint32 getOpenFlags(OpenFlags options);
    MPQFileInfo getFileInfo_p(void *);
    Error lastError(int errorCode); // converts StormLib error to our enum
    bool openFile(const QString & name, void ** hFile);
    void setLastError();
    void unsetError();
};

#endif // QMPQARCHIVE2_H
