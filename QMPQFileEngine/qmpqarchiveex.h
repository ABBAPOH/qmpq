#ifndef QMPQARCHIVEEX_H
#define QMPQARCHIVEEX_H

#include "qmpqarchive2.h"
#include "QMPQFileEngine_global.h"

class Node;
class QMPQArchiveExPrivate;

class QMPQFILEENGINESHARED_EXPORT QMPQArchiveEx : public QMPQArchive2
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMPQArchiveEx)

public:
    explicit QMPQArchiveEx(QObject *parent = 0);
    ~QMPQArchiveEx();

    bool openArchive(const QString & name, OpenFlags flags = OpenFlagNone, QByteArray listfile = QByteArray());
    bool closeArchive();

    bool add(const QString & file, const QString & path);
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
    void initialize(QStringList listfile);
    Node * mkNode(const QString & path, bool createParentDirectories);
    Node * node(const QString & path) const;
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
