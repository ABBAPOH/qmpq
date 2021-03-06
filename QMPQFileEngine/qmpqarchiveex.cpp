#include "qmpqarchiveex.h"

#include "mpqfileinfo.h"
#include "mpqfileinfoiterator.h"

#include <QDebug>

typedef QMPQArchiveExPrivate::Key Key;
class Node
{
public:
    Node(Node *parent = 0, bool isDir = true)
    {
        m_parent = parent;
        if (m_parent) {
            m_parent->childItems.append(this);
        }
        m_isDir = isDir;
    }

    virtual ~Node()
    {
        if (m_parent)
            m_parent->childItems.removeAll(this);
        qDeleteAll(childItems);
    }

    inline QString name() const { return m_name; }
    inline void setName(const QString & name)
    {
        m_name = name;
    }

    inline QString path() const { return m_path; }
    inline void setPath(const QString & path)
    {
        m_path = path;
    }

    inline bool isDir() const { return m_isDir; }
    inline Node * parent() const { return m_parent; }

    inline MPQFileInfo info() const { return m_info; }

    QList<Node*> childItems;
    Node *m_parent;

    bool m_isDir;
    QString m_name;
    QString m_path;
    MPQFileInfo m_info;
};

QMPQArchiveExPrivate::QMPQArchiveExPrivate()
{
    rootNode = new Node(0, true);
    hash.insert(Key(""), rootNode);
}

QMPQArchiveExPrivate::~QMPQArchiveExPrivate()
{
    delete rootNode;
}

void QMPQArchiveExPrivate::clear()
{
    hash.clear();
    //not to lose root after cleaning
    hash.insert(Key(""), rootNode);
    qDeleteAll(rootNode->childItems);
    rootNode->childItems.clear();
//    indexHash.clear();
//    m_listFile.clear();
}


QMPQArchiveEx::QMPQArchiveEx(QObject *parent) :
    QMPQArchive(parent), d_ptr(new QMPQArchiveExPrivate())
{
}

QMPQArchiveEx::~QMPQArchiveEx()
{
    delete d_ptr;
}

bool QMPQArchiveEx::open(const QString & name, OpenFlags flags, QByteArray listfile)
{
    if (isOpened())
        close();
    bool result = QMPQArchive::open(name, flags);
    if (result) {
        initialize(QString(listfile).split("\r\n"));
    }

    return result;
}

bool QMPQArchiveEx::close()
{
    Q_D(QMPQArchiveEx);

//    qDebug("QMPQArchive::closeArchive");
//    if (d->m_updateOnClose)
//        updateListFile();

    d->clear();
    return QMPQArchive::close();
}

bool QMPQArchiveEx::add(const QByteArray & data, const QString & archivedName, FileFlags flags, CompressionFlags compression)
{
    bool result = QMPQArchive::add(data, archivedName, flags, compression);
    if (result) {
        initFile(fileInfo(archivedName));
    }
    return result;
}

bool QMPQArchiveEx::add(const QString & file, const QString & archivedName, FileFlags flags, CompressionFlags compression)
{
    bool result = QMPQArchive::add(file, archivedName, flags, compression);
    if (result) {
        initFile(fileInfo(archivedName));
    }
    return result;
}

//QStringList QMPQArchiveEx::entryList(const QString & name)
//{
//    QStringList result;
//    Node * node = this->node(name);
//    foreach (Node * child, node->childItems) {
//        result.append(child->name());
//    }
//    return result;

//}

QStringList QMPQArchiveEx::dirList(const QString & name)
{
    QStringList result;
    Node * node = this->node(name);
    foreach (Node * child, node->childItems) {
        if (child->isDir())
            result.append(child->name());
    }
    return result;

}

QList<MPQFileInfo> QMPQArchiveEx::entryList(const QString & name)
{
    QList<MPQFileInfo> result;
    Node * node = this->node(name);
    foreach (Node * child, node->childItems) {
        if (!child->isDir())
            result.append(child->info());
    }
    return result;

}

QList<MPQFileInfo> QMPQArchiveEx::entryList()
{
    return d_func()->infoList;
}

bool QMPQArchiveEx::exists(const QString & path)
{
    return node(path);
}

bool QMPQArchiveEx::isDir(const QString & path)
{
    Node * item = node(path);
    return item && item->isDir();
}

bool QMPQArchiveEx::mkdir(const QString & path, bool createParentDirectories)
{
    return mkNode(path, QLocale(QLocale::C), createParentDirectories);
}

bool QMPQArchiveEx::rename(const QString & oldName, const QString & newName)
{
    return rename(node(oldName), newName);
}

bool QMPQArchiveEx::remove(const QString & name)
{
    return remove(node(name));
}

qint64 QMPQArchiveEx::size(const QString &file)/* const*/
{
    MPQFileInfo info = fileInfo(file);

    if (info.isValid())
        return info.fileSize();
    else
        return -1;
}

QString QMPQArchiveEx::getFileName(const QString & fullPath) const
{
    int index = fullPath.lastIndexOf("\\");
    return fullPath.mid(index + 1);
}

QString QMPQArchiveEx::getFilePath(const QString & fullPath) const
{
    int index = fullPath.lastIndexOf("\\");
    if (index == -1)
        return "";
    else
        return fullPath.left(index);
}

void QMPQArchiveEx::initFile(const MPQFileInfo & info)
{
    Node * node = mkNode(info.name(), info.locale(), true);
    node->m_isDir = false;
    node->m_info = info;
}

void QMPQArchiveEx::initialize(QStringList listfile)
{
    Q_D(QMPQArchiveEx);

    MPQFileInfoIterator * iterator = beginFileInfoList(listfile, true);
    d->infoList.clear();

    while (iterator->hasNext()) {
         MPQFileInfo info = iterator->next();
         initFile(info);
         d->infoList.append(info);
    }

    delete iterator;
}

Node * QMPQArchiveEx::mkNode(const QString & path, const QLocale & locale, bool createParentDirectories)
{
    Q_D(QMPQArchiveEx);

    if (d->hash.contains(Key(path, locale))) {
        return d->hash.value(Key(path, locale));
    } else {
        const QString & parentPath = getFilePath(path);
        Node * parent = node(parentPath);
        if (!parent && !createParentDirectories)
            return false;
        qDebug() << path << parentPath;
        parent = mkNode(parentPath, QLocale(QLocale::C), true);
        Node * item = new Node(parent, true);
        d->hash.insert(Key(path, locale), item);
        item->setName(getFileName(path));
        item->setPath(path);
        return item;
    }
}

Node * QMPQArchiveEx::node(const QString & path) const
{
    Q_D(const QMPQArchiveEx);

    if (path == "") {
        return d->rootNode;
    }

    if (d->hash.contains(Key(path, locale()))) { // first try to find node in current locale
        return d->hash.value(Key(path, locale()));
    } else if (d->hash.contains(path)) { // else we use node in neutral locale
        return d->hash.value(path);
    } else {
        return 0;
    }
}

//bool QMPQArchiveEx::add(const QString & file, Node * parent)
//{
//    Q_D(QMPQArchive);
//    //    qDebug(QMPQArchive::add");
//    if (!parent) {
//        return false;
//    }

//    if (!parent->isDir()) {
//        return false;
//    } else {
//        QString name = parent->data(FullPath).toString();
//        name = (name == "" ? "" : name + "\\") + QFileInfo(file).fileName();
//        //            qDebug () << "adding file " << file << " as " << name;
////        d->addLocalFile(file, name);
//        QMPQArchive::add(file, name);
//        initFile(fileInfo(newName));

////        d->initFile(name);
////        d->m_listFile << name;
//    }
//    return true;
//}

bool QMPQArchiveEx::rename(Node * node, const QString & newName)
{
    Q_D(QMPQArchiveEx);
//    qDebug("QMPQArchive::rename");
    if (!node) {
        return false;
    }

//    d->setUpdateOnClose();

    const QString & oldName = node->path();
//    qDebug() << oldName << newName;

    if (d->hash.contains(Key(newName, locale()))) {
        //file or folder already exists
        return false;
    }

    bool isDir = node->isDir();
    if (isDir) {
//        return false;
        QString name = newName;
        if (name != "" ) {
            name += '\\';
        }
        foreach (Node * child, node->childItems) {
            rename(child, name + child->name());
        }
    }

    QString path;
    QString name;
    int index = newName.lastIndexOf('\\');
    if (index != -1) {
        path = newName.left(index);
        name = newName.mid(index+1);
    } else {
        name = newName;
    }
    //    qDebug() << "path: " << path << "name: " << name;
    delete node;
    d->hash.remove(Key(oldName, locale()));

//    d->m_listFile.removeAll(oldName);
//    d->m_listFile.append(newName);
    if (isDir) {
        mkdir(newName, true);
        node = this->node(newName);
    } else {
        bool result = QMPQArchive::rename(oldName, newName);
        initFile(fileInfo(newName));

        return result;
    }
    return true;
}

bool QMPQArchiveEx::remove(Node * node)
{
    Q_D(QMPQArchiveEx);
    if (!node)
        return false;

//    d->setUpdateOnClose();

    QString path = node->path();
    if (node->isDir()) {
        foreach (Node * child, node->childItems) {
            remove(child);
        }
    } else {
        bool result = QMPQArchive::remove(path);
        if (!result)
            return false;
    }
    delete node;
//    d->m_listFile.removeAll(path);
    d->hash.remove(Key(path, locale()));
    return true;
}


