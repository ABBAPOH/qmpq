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

    bool add(const QByteArray & data, const QString & archivedName, FileFlags flags = None, CompressionFlags compression = NoCompression);
    bool add(const QString & file, const QString & archivedName, FileFlags flags = None, CompressionFlags compression = NoCompression);
//    QStringList entryList(const QString & name);
    QList<MPQFileInfo> entryList(const QString & name);
    QList<MPQFileInfo> entryList();
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
    Node * mkNode(const QString & path, const QLocale & locale, bool createParentDirectories);
    Node * node(const QString & path) const;
    bool rename(Node * node, const QString & newName);
    bool remove(Node * node);
};

#include <QHash>

class QMPQArchiveExPrivate
{
public:
    class Key :public QPair<QString, QString>
    {
    public:
        Key(const QString & name, const QString & localeName) :
                QPair<QString, QString>(name, localeName)
        {
        }

        Key(const QString & name, const QLocale & locale) :
                QPair<QString, QString>(name, locale == QLocale(QLocale::C) ? "" : locale.name())
        {
        }

        // constructor is NOT declared as explicit to simpify code
        Key(const QString &name) :
                QPair<QString, QString>(name, "")
        {
        }
    };
    QMPQArchiveExPrivate();
    ~QMPQArchiveExPrivate();
    void clear();

    Node * rootNode;
//    QHash<QString, Node*> hash;
    QHash<Key, Node*> hash;
    QList<MPQFileInfo> infoList;
};

#endif // QMPQARCHIVEEX_H
