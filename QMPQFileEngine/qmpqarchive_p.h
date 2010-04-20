#ifndef QMPQARCHIVE_P_H
#define QMPQARCHIVE_P_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include "qmpqerror.h"

class TreeItem;

class QMPQArchive;
class QMPQArchivePrivate
{
    Q_DECLARE_PUBLIC(QMPQArchive);
public:
    QMPQArchivePrivate(QMPQArchive * q);
    ~QMPQArchivePrivate();
    bool newArchive(const QString & name);
    bool openArchive(const QString & name/*, QByteArray listfile = QByteArray()*/);
    bool closeArchive();
    void clear();
    void initialize(QStringList listfile);
    void initFile(const QString & file);
    void initFile(const char * fileName, quint32 searchScope= 0);
    QByteArray readFile(const QString &file);
    bool addLocalFile(const QString &localFile, const QString &file);
    bool remove(const QString &file);
    bool rename(const QString &oldName, const QString &newName);
    TreeItem * mkfile(const QString & path);
    void mkdir(const QString & path);
    TreeItem * treeItem(const QString & path);
    void setUpdateOnClose() { m_updateOnClose = true; } // prevents updating listfile when archive is used read-only

    QMap<QString, TreeItem*> hash;
    QMap<QString, int> indexHash;
    TreeItem *m_rootItem;
    QStringList m_listFile;
    QString m_file;
    void * mpq; //qfmpq archive pointer;
    QMPQError m_lastError;
    bool m_updateOnClose;
    bool m_isOpened;
private:
    QMPQArchive * q_ptr;
};

#endif // QMPQARCHIVE_P_H
