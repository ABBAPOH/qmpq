#ifndef QMPQARCHIVEEX_H
#define QMPQARCHIVEEX_H

#include "qmpqarchive.h"
#include "QMPQFileEngine_global.h"

class Node;
class QMPQArchiveExPrivate;

class QMPQFILEENGINESHARED_EXPORT QMPQArchiveEx : public QMPQArchive
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMPQArchiveEx)

public:
    explicit QMPQArchiveEx(QObject *parent = 0);
    ~QMPQArchiveEx();

    bool open(const QString & name, OpenFlags flags = OpenFlagNone, QByteArray listfile = QByteArray());
    bool close();

//    bool add(const QString & file, const QString & path);
    QStringList entryList(const QString & name);
    bool exists(const QString & path);
    bool isDir(const QString & path);
    bool mkdir(const QString & path, bool createParentDirectories);
    bool rename(const QString & oldName, const QString & newName);
    bool remove(const QString & name);
    qint64 size(const QString &file)/* const*/;

signals:

public slots:

protected:
    QMPQArchiveExPrivate * d_ptr;
private:
    QString getFileName(const QString & path) const;
    QString getFilePath(const QString & path) const;
    void initFile(const MPQFileInfo & info);
    void initialize(QStringList listfile);
    Node * mkNode(const QString & path, bool createParentDirectories);
    Node * node(const QString & path) const;
    bool rename(Node * node, const QString & newName);
    bool remove(Node * node);
};

#include <QHash>

class QMPQArchiveExPrivate
{
public:
    QMPQArchiveExPrivate();
    ~QMPQArchiveExPrivate();
    void clear();

    Node * rootNode;
    QHash<QString, Node*> hash;
};

#endif // QMPQARCHIVEEX_H
