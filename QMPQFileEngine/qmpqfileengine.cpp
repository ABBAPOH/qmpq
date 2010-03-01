#include "qmpqfileengine.h"
#include "qmpqfileengine_p.h"

#include "mpqfileengineiterator.h"
#include "treeitem.h"
#include "qmpqfileenginehandler.h"

#include <QtCore/QDirIterator>
#include <QtCore/QDateTime>
#include <QDebug>

QString QMPQFileEnginePrivate::getArchiveFilePath(const QString & path)
{
//    QString result = path;
    foreach (QString format, QMPQFileEngine::supportedFormats()) {
        int index = path.toLower().lastIndexOf('.' + format);
        if (index != -1) {
            return path.mid(0, index+format.length() + 1);
        }
    }
    return "";
}

QMPQFileEngine::QMPQFileEngine()
{
//    qDebug() << "QMPQFileEngine::QMPQFileEngine";
}

QMPQFileEngine::QMPQFileEngine(const QString & file)
    : QAbstractFileEngine(), d_ptr(new QMPQFileEnginePrivate(this))
{
    Q_D(QMPQFileEngine);
//    qDebug() << "QMPQFileEngine::QMPQFileEngine" << file;
    setFileName(file);
}

QMPQFileEngine::~QMPQFileEngine()
{
    Q_D(QMPQFileEngine);
//    qDebug() << "QMPQFileEngine::~QMPQFileEngine";
    SharedMPQArchive::releaseInstance(d->archiveFilePath);
}

QAbstractFileEngine::Iterator * QMPQFileEngine::beginEntryList(QDir::Filters filters, const QStringList & filterNames)
{
    Q_D(QMPQFileEngine);
//    qDebug() << "QMPQFileEngine::beginEntryList";
    return new MPQFileEngineIterator(filters, filterNames, d->archive->treeItem(d->innerPath));
}

bool QMPQFileEngine::caseSensitive() const
{
//    qDebug() << "QMPQFileEngine::caseSensitive";
    return false;
}

bool QMPQFileEngine::close()
{
    Q_D(QMPQFileEngine);
//    qDebug() << "QMPQFileEngine::close";

    if (d->openMode & QIODevice::WriteOnly) {
        QString filepath = d->innerPath;
        filepath = filepath.mid(filepath.lastIndexOf('\\') + 1);
        QString tempPath = QDir::tempPath() + "/" + filepath;
        QFile file(tempPath);
        file.open(QFile::WriteOnly);
        file.write(d->fileData);
//        qDebug () << d->fileData;
        file.close();
        if (!d->archive->remove(d->innerPath)) {
//            return false;
            qDebug() << "QMPQFileEngine::close - can't remove";
        }
//        qDebug() << tempPath << d->innerPath;
        if (!d->archive->add(tempPath, d->innerPath)) {
            return false;
            qDebug() << "QMPQFileEngine::close - can't add";
        }
        file.remove();
    }

    d->openMode = QIODevice::NotOpen;
    d->fileData.clear();
    d->offset = 0;

    return true;
}

bool QMPQFileEngine::copy(const QString & newName)
{
    qDebug() << "QMPQFileEngine::copy" << newName;
    open(QIODevice::ReadOnly);
    QFile newFile(newName);
    newFile.open(QFile::WriteOnly);
    newFile.write(d_func()->fileData);
    newFile.close();
    close();
    return true;
}

QStringList QMPQFileEngine::entryList(QDir::Filters filters, const QStringList & filterNames) const
{
//    qDebug() << "QMPQFileEngine::entryList";
    return QAbstractFileEngine::entryList(filters, filterNames);
}

QAbstractFileEngine::FileFlags QMPQFileEngine::fileFlags(FileFlags type) const
{
//    qDebug() << "QMPQFileEngine::fileFlags";
    Q_UNUSED(type);
    QAbstractFileEngine::FileFlags result = 0;
    Q_D(const QMPQFileEngine);
    QFileInfo * info = getNativeFileInfo(d->archiveFilePath);
    if (!info->exists()) {
        delete info;
        return result;
    }
    delete info;
    result |= d->archive->isDir(d->innerPath) ? QAbstractFileEngine::DirectoryType : QAbstractFileEngine::FileType;
    if (d->archive->treeItem(d->innerPath))
        result |= QAbstractFileEngine::ExistsFlag;
    result |= QAbstractFileEngine::ReadOwnerPerm;
    result |= QAbstractFileEngine::WriteOwnerPerm;
    result |= QAbstractFileEngine::ReadUserPerm;
    result |= QAbstractFileEngine::WriteUserPerm;
    result |= QAbstractFileEngine::ReadGroupPerm;
    result |= QAbstractFileEngine::WriteGroupPerm;
    result |= QAbstractFileEngine::ReadOtherPerm;
    result |= QAbstractFileEngine::WriteOtherPerm;
    return result;
}

QString QMPQFileEngine::fileName(FileName file) const
{
    Q_D(const QMPQFileEngine);
//    qDebug() << "QMPQFileEngine::fileName" << file << d->fileName;
    if (file == QAbstractFileEngine::DefaultName) {
        return d->fileName;
    }
    if (file == QAbstractFileEngine::BaseName) {
        return d->baseName;
    }
    if (file == QAbstractFileEngine::PathName)
        return d->filePath.left(d->filePath.lastIndexOf('/')+1);
    return d->filePath;
}

QDateTime QMPQFileEngine::fileTime( FileTime time) const
{
//    qDebug() << "QMPQFileEngine::fileTime" << time;
    return QAbstractFileEngine::fileTime(time);
}

bool QMPQFileEngine::flush()
{
//    d_func()->fileData.clear();
    return true;
}

bool QMPQFileEngine::isRelativePath() const
{
//    qDebug() << "QMPQFileEngine::isRelativePath" << (d_func()->filePath.at(0) != '/');
//    return d_func()->filePath.at(0) == '/';
    return !d_func()->archiveFilePath.contains('/');
//    return false;
}

bool QMPQFileEngine::mkdir(const QString & dirName, bool createParentDirectories) const
{
//    qDebug() << "QMPQFileEngine::mkdir";
    Q_D(const QMPQFileEngine);
    return d->archive->mkdir(dirName.mid(d->archiveFilePath.length() + 1), createParentDirectories);
}

bool QMPQFileEngine::open(QIODevice::OpenMode mode)
{
//    qDebug() << "QMPQFileEngine::open";
    Q_D(QMPQFileEngine);
    d->openMode = mode;
    d->offset = 0;
    if (mode & QIODevice::ReadOnly) {
        d->fileData = d->archive->read(d->innerPath);
    }
    if (mode & QIODevice::WriteOnly) {
        d->fileData.clear();
    }
    return true;
}

QString QMPQFileEngine::owner(FileOwner owner) const
{
//    qDebug() << "QMPQFileEngine::owner";
    return "";
}

uint QMPQFileEngine::ownerId(FileOwner owner) const
{
//    qDebug() << "QMPQFileEngine::ownerId";
    return -2;
}

qint64 QMPQFileEngine::pos() const
{
//    qDebug() << "QMPQFileEngine::pos";

    return d_func()->offset;
}

//qint64 QMPQFileEngine::seek() const
//{
//    qDebug() << "QMPQFileEngine::seek";
//    return 0;
//}

qint64 QMPQFileEngine::read(char* data, qint64 maxlen)
{
    Q_D(QMPQFileEngine);
//    qDebug() << "QMPQFileEngine::read";
    if (!d->openMode & QIODevice::ReadOnly)
        return 0;
    qint64 ds = size() - d->offset;
    if (maxlen > ds)
        maxlen = ds;
    if (maxlen <= 0)
        return 0;
    memcpy(data, d->fileData.constData() + d->offset, maxlen);
    d->offset += maxlen;
    return maxlen;
}

bool QMPQFileEngine::remove()
{
//    qDebug() << "QMPQFileEngine::remove";
    return d_func()->archive->remove(d_func()->innerPath);
}

bool QMPQFileEngine::rename(const QString & newName)
{
    qDebug() << "QMPQFileEngine::rename" << newName;
    Q_D(const QMPQFileEngine);
    if (d->innerPath=="") {
        #warning TODO: fix it
        QMPQFileEngineHandler::setLocked(true);
        QFile file(d->archiveFilePath);
        bool result = file.rename(newName);
        QMPQFileEngineHandler::setLocked(false);
        return result;
    } else
        return d->archive->rename(d->innerPath, newName.mid(newName.lastIndexOf('/') + 1));
//    QFile newFile(newName);
//    newFile.open(QFile::WriteOnly);
//    open(QIODevice::ReadOnly);
//    newFile.write(d->fileData);
//    newFile.close();
//    close();
//    return remove();
}

bool QMPQFileEngine::rmdir(const QString & dirName, bool recurseParentDirectories) const
{
//    qDebug() << "QMPQFileEngine::rmdir";
    Q_D(const QMPQFileEngine);
    if (!d->archive->isDir(dirName))
        return false;
    return d_func()->archive->remove(dirName);
}

void QMPQFileEngine::setFileName(const QString & fileName)
{
    Q_D(QMPQFileEngine);
    d->fileName = fileName;
    const QString file = QDir::fromNativeSeparators(fileName);
    if (d->filePath == file)
        return;
//    qDebug() << "QMPQFileEngine::setFileName" << d->filePath << file;
    SharedMPQArchive::releaseInstance(d->archiveFilePath);
    d->filePath = file;
    d->archiveFilePath = d->getArchiveFilePath(d->filePath);
    d->baseName = file.mid(file.lastIndexOf('/') + 1);
//    d->innerPath = file.mid(d->archiveFilePath.length() + 1).replace('/', '\\');
    d->innerPath = file.mid(d->archiveFilePath.length() + 1);
    d->innerPath = d->innerPath.replace("/", "\\");
    d->archive = SharedMPQArchive::instance(d->archiveFilePath);
//    qDebug() << d->archiveFilePath;
//    qDebug() << d->innerPath;

}

bool QMPQFileEngine::setPermissions(uint perms)
{
//    qDebug() << "QMPQFileEngine::setPermissions";
    return false;
}

bool QMPQFileEngine::setSize(qint64 size)
{
//    qDebug() << "QMPQFileEngine::setSize";
    return false;
}

qint64 QMPQFileEngine::size() const
{
    Q_D(const QMPQFileEngine);
//    qDebug() << "QMPQFileEngine::size";
//    ((QObject*)0)->parent();
    return d->archive->size(d->innerPath);
}

qint64 QMPQFileEngine::write(const char * data, qint64 maxlen)
{
    Q_D(QMPQFileEngine);
    qDebug() << "QMPQFileEngine::write" << data << maxlen;
    if (!d->openMode & QIODevice::WriteOnly)
        return 0;
    if (maxlen <= 0)
        return 0;
    d->fileData.append(data, maxlen);
    d->offset += maxlen;
    return maxlen;
}

QFile * QMPQFileEngine::getNativeFile(QString path) const
{
    QMPQFileEngineHandler::setLocked(true);
    QFile * result = new QFile(path);
    QMPQFileEngineHandler::setLocked(false);
    return result;
}

QFileInfo * QMPQFileEngine::getNativeFileInfo(QString path) const
{
    QMPQFileEngineHandler::setLocked(true);
    QFileInfo * result = new QFileInfo(path);
    QMPQFileEngineHandler::setLocked(false);
    return result;
}
