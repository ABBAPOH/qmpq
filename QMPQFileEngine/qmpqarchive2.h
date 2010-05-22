#ifndef QMPQARCHIVE2_H
#define QMPQARCHIVE2_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "mpqfileinfo.h"
#include "mpqfileinfoiterator.h"

class QMPQArchive2Private;
class QMPQArchive2 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMPQArchive2)
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
        NoError = 0
    };
    Q_ENUMS(Error);

    enum OpenFlag
    {
        OpenFlagNone = 0
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

    enum Attribute {
        CRC32 = 1,
        FileTime = 2,
        MD5 = 4
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)
    Q_FLAGS(Attributes)

    explicit QMPQArchive2(QObject *parent = 0);
    ~QMPQArchive2();

    bool addFile(const QString & file, const QString & archivedName, FileFlags flags = None, CompressionFlags compression = NoCompression/*, DWORD dwCompressionNext = 0xFFFFFFFF*/);
    bool addFile(const QByteArray & data, const QString & archivedName, FileFlags flags = None, CompressionFlags compression = NoCompression/*, DWORD dwCompressionNext = 0xFFFFFFFF*/);
    MPQFileInfoIterator * beginFileInfoList(const QStringList & listfile = QStringList(), bool includeUnknowns = true);
    bool createArchive(const QString & name, int hashTableSize, CreateFlags flags = ArchiveV1);
    bool createAttributes(Attributes);
    bool closeArchive();
    bool compact();
    QString errorString();
    QString errorString(Error code);
    MPQFileInfo fileInfo(const QString & file);
    MPQFileInfo fileInfo(quint32 index);
    bool flush();
    bool hasFile(const QString & file);
    bool isOpened() const;
    Error lastError();
    bool openArchive(const QString & name, OpenFlags flags = OpenFlagNone);
    QByteArray readFile(const QString &file);
    bool removeFile(const QString & fileName);
    bool renameFile(const QString & oldFileName, const QString & newFileName);
    VerifyArchiveError verifyArchive();
    bool updateFileAttributes(const QString & fileName);

    Attributes attributes() const;
    bool setAttributes(Attributes attributes);

    quint32 blockTableSize() const;

    quint32 filesCount() const;

    quint32 hashTableSize() const;
    bool setHashTableSize(quint32 size);

    quint32 sectorSize() const;

signals:
    void error();
    void compactProgressChanged(QMPQArchive2::CompactOperation op, qint64 bytesProcessed, qint64 bytesTotal);
    void addFileProgressChanged(quint32 bytesWritten, quint32 totalBytes, bool finalCall);

public slots:

protected:
    QMPQArchive2Private * d_ptr;

private:
    static void addFileCallBack(void *, quint32 bytesWritten, quint32 totalBytes, bool finalCall);
    static void compactCallBack(void *, int, qint64 * bytesProcessed, qint64 * bytesTotal);
    void getArchiveInfo();
    quint32 getAddFileOptionFlags(FileFlags options);
    quint32 getCompressionFlags(CompressionFlags types);
    MPQFileInfo getFileInfo_p(void *);
//    Error GetLastError(); // wrapper aroung StormLib's GetLastError()
    Error lastError(int errorCode); // converts StormLib error to our enum
    bool openFile(const QString & name, void ** hFile);
    void setLastError();
};

#endif // QMPQARCHIVE2_H
