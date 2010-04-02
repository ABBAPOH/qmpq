#include "qmpqarchive.h"
#include "qmpqarchive_p.h"

#include <QFile>
#include <QDir>
//#include <QDesktopServices>
#include <StormLib/Stormlib.h>

#include "treeitem.h"

#include <QDebug>

QMPQArchivePrivate::QMPQArchivePrivate(QMPQArchive * q)
    :  mpq(0), m_updateOnClose(false), m_isOpened(false), q_ptr(q)
{
}

QMPQArchivePrivate::~QMPQArchivePrivate()
{
    if (mpq) {
        closeArchive();
    }
    delete m_rootItem;
}

bool QMPQArchivePrivate::openArchive(const QString & name/*, QByteArray listfile*/)
{
    if (mpq) {
        closeArchive();
    }

#warning under Linux toLower() may cause bug with case-sensitive files
    if (!SFileCreateArchiveEx(name.toLower().toLocal8Bit().data(), OPEN_EXISTING, 0, &mpq)) {
        m_lastError = GetLastError();
        qWarning() << "can't open archive: "<< m_lastError.errorMessage();
        m_isOpened = false;
        return false;
    }

    m_isOpened = true;
    return true;
}

bool QMPQArchivePrivate::closeArchive()
{
    if (!mpq) {
        m_isOpened = false;
        return true;
    }
    bool result = SFileCloseArchive(mpq);
    if (!result) {
        m_lastError = GetLastError();
        qWarning() << "can't close archive: "<< m_lastError.errorMessage();
        return false;
    }
    mpq = 0;
    m_isOpened = false;
    return true;
}

void QMPQArchivePrivate::getListFile(QStringList listfile)
{
//    qDebug() << "QMPQArchive::getListFile()";
    QList<int> indexes;
    int nError = ERROR_SUCCESS;

    if (nError == ERROR_SUCCESS)
    {
        SFILE_FIND_DATA sf;
        void * hFind;
        bool bFound = true;

        hFind = SFileFindFirstFile(mpq, "*", &sf, 0);
        if (!hFind)
            return;
//        Q_ASSERT(hFind);

        while (hFind != 0 && bFound != false)
        {
            listfile << sf.cFileName;
            bFound = SFileFindNextFile(hFind, &sf);
        }
    }

    listfile << "(listfile)" << "(attributes)";

    foreach(const QString &file, listfile) {
        int index = getFileInfo(file.toLocal8Bit().data());
        if (index != -1) {
            indexes.append(index);
        }
    }

    int count = SFileGetFileInfo(mpq, SFILE_INFO_BLOCK_TABLE_SIZE);
    for (int i = 0; i < count; i++)
    {
        if (indexes.contains(i))
            continue;
        getFileInfo((const char *)i, SFILE_OPEN_BY_INDEX);
    }
}

int QMPQArchivePrivate::getFileInfo(const char * fileName, quint32 searchScope)
{
    Q_Q(QMPQArchive);
//    qDebug() << "QMPQArchive::getFileInfo";
    void * hFile;
    TMPQFile * hf;
    int index = -1;
    if (SFileOpenFileEx(mpq, fileName, searchScope, &hFile))
    {
        hf = (TMPQFile *)hFile;
        index = SFileGetFileInfo( hFile, SFILE_INFO_BLOCKINDEX);

        QString name;
        if (searchScope == SFILE_OPEN_BY_INDEX) {
            char buffer[255];
            SFileGetFileName(hFile, buffer);
            name = QString(buffer);
        } else {
            name = QString(fileName);
        }
//        qDebug () << "    " << name;
        if (name != "") {
            m_listFile << name;
        }
        //TreeItem * item = treeItem(name, true);
        TreeItem * item = q->mkfile(name);
        qlonglong fileSize = SFileGetFileInfo(hFile, SFILE_INFO_FILE_SIZE);
        qlonglong compressedSize = SFileGetFileInfo(hFile, SFILE_INFO_COMPRESSED_SIZE);
        item->setData(QMPQArchive::FullSize, QVariant(fileSize));
        item->setData(QMPQArchive::CompressedSize, QVariant(compressedSize));
//        item->index = index;

        indexHash.insert(name, index);
        if (searchScope == SFILE_OPEN_BY_INDEX)
            item->setUnknown(true);

        SFileCloseFile(hFile);
    }
    return index;
}

QByteArray QMPQArchivePrivate::readFile(const QString &file)
{
//    qDebug() << "QMPQArchivePrivate::readFile" << file;
    void * filePointer = 0;
    bool result = SFileOpenFileEx(mpq, (const char*)indexHash.value(file), SFILE_OPEN_BY_INDEX, &filePointer);
    if (!result) {
        m_lastError = GetLastError();
        qWarning() << "can't extract file: errcode = " << m_lastError.errorCode() << m_lastError.errorMessage();
        return QByteArray();
    }
    unsigned size = SFileGetFileSize(filePointer, 0);
    char buffer[size];
    result = SFileReadFile(filePointer, buffer, size, 0, 0);
    if (!result) {
        m_lastError = GetLastError();
        qWarning() << "SFileReadFile failed" << m_lastError.errorMessage();
        return QByteArray();
    }
    return QByteArray(buffer, size);
}

bool QMPQArchivePrivate::addLocalFile(const QString &localFile, const QString &file)
{
    setUpdateOnClose();
    bool result = SFileAddFile(mpq,
                        localFile.toLocal8Bit().data(),
                        file.toLocal8Bit().data(),
                        0);
    if (!result) {
        m_lastError = GetLastError();
        qWarning() << "can't add file: " << m_lastError.errorMessage();
        return false;
    }
    return true;
}

bool QMPQArchivePrivate::remove(const QString &file)
{
    setUpdateOnClose();

    if (!SFileRemoveFile(mpq, (const char *)indexHash.value(file), SFILE_OPEN_BY_INDEX)) {
        m_lastError = GetLastError();
        qWarning()  << "can't remove file: " << m_lastError.errorMessage();
        return false;
    }
    indexHash.remove(file);
    return true;
}

bool QMPQArchivePrivate::rename(const QString &oldName, const QString &newName)
{
    setUpdateOnClose();
    bool res = SFileRenameFile(mpq, oldName.toLocal8Bit().data(), newName.toLocal8Bit().data());
//    qDebug() << "QMPQArchivePrivate::rename" << oldName << newName << res;
    if (!res) {
        m_lastError = GetLastError();
        qWarning() << "can't rename file: " << oldName << GetLastError();
        return false;
    }
    return true;
}

QMPQArchive::QMPQArchive()
    : d_ptr(new QMPQArchivePrivate(this))
{
    Q_D(QMPQArchive);
    d->m_rootItem = new TreeItem(0, true);
    d->mpq = 0;
//    m_rootItem->m_dir = true;
    d->hash.insert("", d->m_rootItem);
}

QMPQArchive::~QMPQArchive()
{
//    qDebug("QMPQArchive::~QMPQArchive");
    closeArchive();
    delete d_ptr;
//    if (mpq) {
//        closeArchive();
//    }
//    delete m_rootItem;
}

//bool QMPQArchive::newArchive(const QString & name, int flags, int maximumFilesInArchive)
//{
//    Q_D(QMPQArchive);
//    qDebug() << "newArchive " << name;
//    closeArchive();
//    d->newArchive(name.toLocal8Bit().data());
////    QFile file(name);
////    file.open(QFile::WriteOnly);
////    file.close();
//
////    if ( (int)(mpq = MpqOpenArchiveForUpdate(name.toLocal8Bit().data(), MOAU_CREATE_NEW, maximumFilesInArchive)) == -1 ) {
//    if (SFileCreateArchiveEx(name.toLocal8Bit().data(), CREATE_ALWAYS | flags, maximumFilesInArchive, &mpq)) {
//        //  always returns that file not exists... look why!!
//        m_lastError = GetLastError();
//        qDebug() << "can't create archive: "<< m_lastError.errorMessage();
////        return false;
//    }
//    SFileCloseArchive(mpq);
//    mpq = 0;
//
//    openArchive(name);
//    return true;
//}

bool QMPQArchive::openArchive(const QString & name, QByteArray listfile)
{
    Q_D(QMPQArchive);
//    qDebug() << "QMPQArchive::openArchive" << name;
    bool result = d->openArchive(name/*, listfile*/);
    d->m_file = name;
    d->getListFile(QString(listfile).split("\n"));

    return result;
}

bool QMPQArchive::closeArchive()
{
    Q_D(QMPQArchive);
//    qDebug("QMPQArchive::closeArchive");
    if (d->m_updateOnClose)
        updateListFile();
    d->hash.clear();
    //not to lose root after cleaning
    d->hash.insert("", d->m_rootItem);
    qDeleteAll(d->m_rootItem->childItems);
    d->m_rootItem->childItems.clear();
    bool result = d->closeArchive();
    d->m_listFile.clear();
    return result;
}

bool QMPQArchive::extract(const QString & name, const QString & path)
{
    return extract(treeItem(name), path);
}

QString QMPQArchive::extractToTemp(const QString & name)
{
    return extractToTemp(treeItem(name));
}

bool QMPQArchive::add(const QStringList & files, const QString & path)
{
    return add(files, treeItem(path));
}

bool QMPQArchive::add(const QString & file, const QString & path)
{
//    qDebug() << "QMPQArchive::add" << file << path;
    return add(QStringList() << file, treeItem(QMPQArchive::getFilePath(path)));
}

QByteArray QMPQArchive::read(const QString & file)
{
    return d_ptr->readFile(file);
}

bool QMPQArchive::rename(const QString & oldName, const QString & newName)
{
    return rename(treeItem(oldName), newName);
}

bool QMPQArchive::remove(const QString & name)
{
    return remove(treeItem(name));
}

const QMPQError & QMPQArchive::lastError()
{
    Q_D(const QMPQArchive);
    return d->m_lastError;
}

const QString QMPQArchive::getFileName(const QString & path)
{
    int index = path.lastIndexOf("\\");
    return path.mid(index + 1);
}

const QString QMPQArchive::getFilePath(const QString & fullPath)
{
    int index = fullPath.lastIndexOf("\\");
    if (index == -1)
        return "";
    else
        return fullPath.left(index);
}

bool QMPQArchive::extract(TreeItem * item, const QString & path)
{
    Q_D(QMPQArchive);
    if (item == 0)
        return false;

    if (item->isDir()) {
        const QString & newPath = path + "/" + item->data(Name).toString();
        QDir dir;
        dir.mkpath(newPath);
        foreach (TreeItem * child, item->childItems) {
            bool result = extract(child, newPath);
            if (!result) {
                return false;
            }
        }
    }

    int size = item->data(FullSize).toInt();
    if (size) {
        QString targetPath;
        QString name = item->data(Name).toString();
        QString fullPath = item->data(FullPath).toString();
        if (QFileInfo(path).isDir()) {
            targetPath = path + "/" + name;
        } else {
            targetPath = path;
        }


        QFile file(targetPath);
        file.open(QIODevice::WriteOnly);
        QByteArray bytes = d->readFile(item->data(FullPath).toString());
        file.write(bytes);
        file.close();
    }
    return true;
}

QString QMPQArchive::extractToTemp(TreeItem * item)
{
    Q_ASSERT(item);
    if (!item)
        return "";
//    qDebug() << "QMPQArchive::extractToTemp" << item->data(Name);
    //TODO: fix to make it work on my machine
    //QString path = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
    QString path = QDir::tempPath();
    //Fixes Qt 4.5.3 bug
#ifdef Q_OS_MAC
    if (path == "") path = "/tmp";
#endif
    extract(item, path);
    return path + "/" + item->data(Name).toString();
}

bool QMPQArchive::add(const QStringList & files, TreeItem * parent)
{
    Q_D(QMPQArchive);
//    qDebug(QMPQArchive::add");
    if (!parent) {
        return false;
    }

    d->setUpdateOnClose();

    if (!parent->isDir()) {
        return false;
    } else {
        foreach (QString file, files) {
            QString name = parent->data(FullPath).toString();
            name = (name == "" ? "" : name + "\\") + QFileInfo(file).fileName();
//            qDebug () << "adding file " << file << " as " << name;
            d->addLocalFile(file, name);
            d->getFileInfo(name.toLocal8Bit().data(), 0);
            d->m_listFile << name;
        }
    }
    return true;
}

bool QMPQArchive::rename(TreeItem * item, const QString & newName)
{
    Q_D(QMPQArchive);
//    qDebug("QMPQArchive::rename");
    if (!item) {
        return false;
    }

    d->setUpdateOnClose();

    const QString & oldName = item->data(FullPath).toString();
//    qDebug() << oldName << newName;

    if (d->hash.contains(newName)) {
        //file or filder already exists
        return false;
    }

    bool isDir = item->isDir();
    if (isDir) {
//        return false;
        QString name = newName;
        if (name != "" ) {
            name += '\\';
        }
        foreach (TreeItem * child, item->childItems) {
            rename(child, name + child->data(Name).toString());
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
        TreeItem * oldParent = item->parent();
        oldParent->deleteChild(item);
        if (isDir) {
            mkdir(newName, true);
            item = this->treeItem(newName);
        } else
            item = mkfile(newName);
        d->hash.remove(oldName);
        item->setData(FullPath, newName);
        item->setData(Name, newName.split('\\').last());
        d->m_listFile.removeAll(oldName);
        d->m_listFile.append(newName);

        if (!isDir) {
            bool res = d->rename(oldName, newName);
            d->getFileInfo(newName.toLocal8Bit().data(), 0);

            return res;
        }
        return true;
}

bool QMPQArchive::remove(TreeItem * item)
{
    Q_D(QMPQArchive);
    if (!item)
        return false;

    d->setUpdateOnClose();

    TreeItem * parent = item->parent();
    QString name = item->data(FullPath).toString();
//    qDebug() << "QMPQArchive::remove " << name.toLocal8Bit().data();
    if (item->isDir()) {
        foreach (TreeItem * child, item->childItems) {
            remove(child);
        }
    } else {
        bool result = d->remove(item->data(FullPath).toString());
        if (!result)
            return false;
    }
    parent->deleteChild(item);
    d->m_listFile.removeAll(name);
    d->hash.remove(name);
    return true;
}

bool QMPQArchive::mkdir(const QString & path, bool createParentDirectories)
{
    Q_D(QMPQArchive);

    if (d->hash.contains(path)) {
        return true;
    } else {
        const QString & parentPath = getFilePath(path);
        TreeItem * parent = treeItem(parentPath);
        if (!parent && !createParentDirectories)
            return false;
        mkdir(parentPath);
        parent = treeItem(parentPath);
        TreeItem * item = new TreeItem(parent, true);
        d->hash.insert(path, item);
        item->setData(Name, getFileName(path));
        item->setData(FullPath, path);
        parent->appendChild(item);
    }
    return true;
}

qint64 QMPQArchive::size(const QString &file) const
{
    TreeItem * item = treeItem(file);
//    qDebug() << "QMPQArchive::size()" << file << item;

    if (!item || item->isDir())
        return -1;
    else
        return item->data(QMPQArchive::FullSize).toInt();
}

bool QMPQArchive::detachTree(TreeItem * item)
{
    return item->parent()->remove(item);
}

TreeItem * QMPQArchive::getExistingParent(QString path)
{
    TreeItem * parent = treeItem(path);
    if (!parent)
        return getExistingParent(getFilePath(path));
    return parent;
}

void QMPQArchive::updateListFile()
{
    Q_D(QMPQArchive);
    bool ok = true;
//    qDebug("QMPQArchive::updateListFile");
    QString path = QDir::tempPath();
    path += "/(listfile)";
    QFile file(path);
//    qDebug() << d->m_listFile;
    file.open(QIODevice::WriteOnly);
    foreach (QString name, d->m_listFile) {
        file.write((name + "\r\n").toLocal8Bit());
    }
    file.close();
    ok = remove("(listfile)");
    ok = add(QStringList() << path, rootItem());
    if (!ok)
        qWarning() << "can't update (listfile)";
}

bool QMPQArchive::isDir(const QString & dirName)
{
    TreeItem* item = treeItem(dirName);
    if (!item)
        return false;
    return item->isDir();
}

bool QMPQArchive::isOpened()
{
    return d_func()->m_isOpened;
}


QString QMPQArchive::file()
{
    return d_func()->m_file;
}

TreeItem * QMPQArchive::mkfile(const QString & path)
{
    Q_D(QMPQArchive);
    if (d->hash.contains(path)) {
        return d->hash[path];
    } else {
        mkdir(getFilePath(path));
        TreeItem * parent = treeItem(getFilePath(path));
        TreeItem * item = new TreeItem(parent, false);
        d->hash.insert(path, item);
        item->setData(Name, getFileName(path));
        item->setData(FullPath, path);
        parent->appendChild(item);
        return item;
    }
}

TreeItem * QMPQArchive::treeItem(const QString & path) const
{
    Q_D(const QMPQArchive);
    if (path == "") {
        return d->m_rootItem;
    }
    if (d->hash.contains(path)) {
        return d->hash[path];
    } else {
        return 0;
    }
}

TreeItem * QMPQArchive::rootItem()
{
    Q_D(QMPQArchive);
    return d->m_rootItem;
}
