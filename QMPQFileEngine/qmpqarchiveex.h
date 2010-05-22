#ifndef QMPQARCHIVEEX_H
#define QMPQARCHIVEEX_H

#include "qmpqarchive2.h"

class Node;
class QMPQArchiveExPrivate;

class QMPQArchiveEx : public QMPQArchive2
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMPQArchiveEx)

public:
    explicit QMPQArchiveEx(QObject *parent = 0);
    ~QMPQArchiveEx();

    bool openArchive(const QString & name, OpenFlags flags = OpenFlagNone, QByteArray listfile = QByteArray());
    bool closeArchive();

    bool exists(const QString & path);
    bool isDir(const QString & path);
    bool mkdir(const QString & path, bool createParentDirectories);

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
