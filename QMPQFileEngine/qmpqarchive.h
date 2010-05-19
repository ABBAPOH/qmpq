#ifndef QMPQARCHIVE_H
#define QMPQARCHIVE_H

#include "QMPQFileEngine_global.h"

#include <QMap>
#include <QStringList>
#include <QDebug>

#include "QMPQError.h"

class TreeItem;

//#define MPQ_CREATE_ARCHIVE_V1   0x00000000  // Creates archive with size up to 4GB
//#define MPQ_CREATE_ARCHIVE_V2   0x00010000  // Creates archive larger than 4 GB
//#define MPQ_CREATE_ATTRIBUTES   0x00100000  // Also add the (attributes) file

class QMPQArchivePrivate;
class QMPQFILEENGINESHARED_EXPORT QMPQArchive : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMPQArchive);
    friend class QMPQModel;
public:
    enum CreateModeFlag {
        CreateArchiveV1 = 0x00000000,
        CreateArchiveV2 = 0x00010000,
        CreateAttributes = 0x00100000
    };

    enum CompactOperation {
        CheckingFiles = 1,
        CheckingHashTableSize,
        CopyingNonMPQData,
        CompactingFiles,
        ClosingArchive
    };

    QMPQArchive(QObject * parent = 0);
    virtual ~QMPQArchive();

    enum {Name, CompressedSize, FullSize, FullPath};
//    enum {CreateArchiveV1, CreateArchiveV2}
    bool newArchive(const QString & name, int flags = /*MPQ_CREATE_ARCHIVE_V1*/QMPQArchive::CreateArchiveV1, int maximumFilesInArchive = 1024);
    bool openArchive(const QString & name, QByteArray listfile = QByteArray());
    bool closeArchive();
    bool compact();
    bool extract(const QString & name, const QString & path);
    QString extractToTemp(const QString & name);
    bool add(const QStringList & files, const QString & path);
    bool add(const QString & file, const QString & path);
    QByteArray read(const QString & file);
    bool rename(const QString & oldName, const QString & newName);
    bool remove(const QString & name);
    bool mkdir(const QString & path, bool createParentDirectories = true);
    qint64 size(const QString &file) const;
    qint64 compressedSize(const QString &file) const;

    const QMPQError & lastError();
    static const QString getFileName(const QString & path);
    static const QString getFilePath(const QString & fullPath);

    bool exists(const QString & file) { return treeItem(file); }
    bool isDir(const QString & dirName);
    bool isOpened();

    QString file();

//private:
    TreeItem * mkfile(const QString & path);
    TreeItem * mkfile(const QString & path, qlonglong size, qlonglong compressedSize);
    TreeItem * treeItem(const QString & path) const;
    TreeItem * rootItem();// { return m_rootItem; }
    bool extract(TreeItem * item, const QString & path);
    QString extractToTemp(TreeItem * item);
    bool remove(TreeItem * item);
    bool add(const QStringList & files, TreeItem * item);
    bool rename(TreeItem * item, const QString & newName);
    bool detachTree(TreeItem * item);
    TreeItem * getExistingParent(QString path);

//    void getListFile(QStringList listfile);
    void updateListFile();
//    int getFileInfo(const char * fileName, quint32 searchScope= 0);

//private:
//    QMap<QString, TreeItem*> hash;
//    TreeItem *m_rootItem;
//    QStringList m_listFile;
//    void * mpq; //qfmpq archive pointer;
//    QMPQError m_lastError;
protected:
    QMPQArchivePrivate *d_ptr;
signals:
    void compactProcessChanged(QMPQArchive::CompactOperation op, qint64 bytesProcessed, qint64 bytesTotal);
//    {
//        qDebug() << op << bytesProcessed << bytesTotal;
//    }
};

#endif // QMPQARCHIVE_H
