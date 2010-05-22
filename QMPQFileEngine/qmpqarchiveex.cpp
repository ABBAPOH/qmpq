#include "qmpqarchiveex.h"
#include <QDebug>

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
//        m_info = 0;
    }

//    Node(Node * parent, MPQFileInfo info)
//    {
//        m_parent = parent;
//        m_isDir = false;
//        m_info = new MPQFileInfo(info);
//    }

    virtual ~Node()
    {
        if (m_parent)
            m_parent->childItems.removeAll(this);
        qDeleteAll(childItems);
//        delete m_info;
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

    QList<Node*> childItems;
    Node *m_parent;

    bool m_isDir;
    QString m_name;
    QString m_path;
//    MPQFileInfo * m_info;
};

QMPQArchiveExPrivate::QMPQArchiveExPrivate()
{
    rootNode = new Node(0, true);
    hash.insert("", rootNode);
}

QMPQArchiveExPrivate::~QMPQArchiveExPrivate()
{
    delete rootNode;
}

void QMPQArchiveExPrivate::clear()
{
    hash.clear();
    //not to lose root after cleaning
    hash.insert("", rootNode);
    qDeleteAll(rootNode->childItems);
    rootNode->childItems.clear();
//    indexHash.clear();
//    m_listFile.clear();
}


QMPQArchiveEx::QMPQArchiveEx(QObject *parent) :
    QMPQArchive2(parent), d_ptr(new QMPQArchiveExPrivate())
{
}

QMPQArchiveEx::~QMPQArchiveEx()
{
    delete d_ptr;
}

bool QMPQArchiveEx::openArchive(const QString & name, OpenFlags flags, QByteArray listfile)
{
//    Q_D(QMPQArchiveEx);

    if (isOpened())
        closeArchive();
    bool result = QMPQArchive2::openArchive(name, flags);
//    d->m_file = name;
    initialize(QString(listfile).split("\r\n"));

    return result;
}

bool QMPQArchiveEx::closeArchive()
{
    Q_D(QMPQArchiveEx);

//    qDebug("QMPQArchive::closeArchive");
//    if (d->m_updateOnClose)
//        updateListFile();

    d->clear();
    return QMPQArchive2::closeArchive();
}

bool QMPQArchiveEx::add(const QString & file, const QString & path)
{
}

QStringList QMPQArchiveEx::entryList(const QString & name)
{
    QStringList result;
    Node * node = this->node(name);
    foreach (Node * child, node->childItems) {
        result.append(child->name());
    }
    return result;

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
    return mkNode(path, createParentDirectories);
}

bool QMPQArchiveEx::rename(const QString & oldName, const QString & newName)
{
    qDebug("QMPQArchiveEx::rename");
}

bool QMPQArchiveEx::remove(const QString & name)
{
    return remove(node(name));
}

qint64 QMPQArchiveEx::size(const QString &file)/* const*/
{
    MPQFileInfo info = fileInfo(file);
//    qDebug() << "QMPQArchive::size()" << file << item;

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

void QMPQArchiveEx::initialize(QStringList listfile)
{
    MPQFileInfoIterator * iterator = beginFileInfoList(listfile, true);

    while (iterator->hasNext()) {
         MPQFileInfo info = iterator->next();
         Node * node = mkNode(info.name(), true);
         node->m_isDir = false;
    }

    delete iterator;
}

Node * QMPQArchiveEx::mkNode(const QString & path, bool createParentDirectories)
{
    Q_D(QMPQArchiveEx);

    if (d->hash.contains(path)) {
        return d->hash.value(path);
    } else {
        const QString & parentPath = getFilePath(path);
        Node * parent = node(parentPath);
        if (!parent && !createParentDirectories)
            return false;
        parent = mkNode(parentPath, true);
        Node * item = new Node(parent, true);
        d->hash.insert(path, item);
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

    if (d->hash.contains(path)) {
        return d->hash.value(path);
    } else {
        return 0;
    }
}

bool QMPQArchiveEx::remove(Node * node)
{
    Q_D(QMPQArchiveEx);
    if (!node)
        return false;

//    d->setUpdateOnClose();

//    Node * parent = node->parent();
    QString path = node->path();
    qDebug() << "QMPQArchive::remove " << path;
    if (node->isDir()) {
        foreach (Node * child, node->childItems) {
            remove(child);
        }
    } else {
//        bool result = remove(node->data(FullPath).toString());
        bool result = removeFile(path);
        qDebug() << "QMPQArchiveEx::remove" << result;
        if (!result)
            return false;
    }
//    parent->deleteChild(node);
    delete node;
//    d->m_listFile.removeAll(path);
    d->hash.remove(path);
    return true;
}


