#include "qmpqfileengine.h"
#include "qmpqfileengine_p.h"

#include "qmpqarchivecache.h"
#include "mpqfileengineiterator.h"
#include "mpqsettings.h"
#include "qmpqfileenginehandler.h"

#include <QtCore/QDirIterator>
#include <QtCore/QDateTime>

QString QMPQFileEnginePrivate::getArchiveFilePath(const QString & path)
{
    int index = path.length();
    QString currentPath;
    while (index != -1) {
        currentPath = path.left(index);
        foreach (QString suffix, QMPQFileEngineHandler::suffixes()) {
            if (currentPath.endsWith(suffix, Qt::CaseInsensitive)) {
                QFileInfo info(currentPath);
                if (info.isFile() && info.exists()) {
                    return currentPath;
                }
            }
        }
        index = currentPath.lastIndexOf('/');
    }
    return "";
}

QMPQFileEngine::QMPQFileEngine()
    : QAbstractFileEngine(), d_ptr(new QMPQFileEnginePrivate(this))
{
}

QMPQFileEngine::~QMPQFileEngine()
{
    Q_D(QMPQFileEngine);
    QMPQArchiveCache::instance()->remove(d->archiveFilePath);
    delete d_ptr;
}

QMPQArchiveEx * QMPQFileEngine::archive()
{
    return d_func()->archive;
}

QAbstractFileEngine::Iterator * QMPQFileEngine::beginEntryList(QDir::Filters filters, const QStringList & filterNames)
{
    Q_D(QMPQFileEngine);
//    return new MPQFileEngineIterator(filters, filterNames, d->archive->treeItem(d->innerPath));
    return new MPQFileEngineIterator(filters, filterNames, d->archive->entryList(d->innerPath));
}

bool QMPQFileEngine::caseSensitive() const
{
    return false;
}

bool QMPQFileEngine::close()
{
    Q_D(QMPQFileEngine);

    if (d->openMode & QIODevice::WriteOnly) {
        if (!d->archive->remove(d->innerPath)) {
            qDebug() << "QMPQFileEngine::close - can't remove";
        }

        QString suffix = QFileInfo(fileName(AbsoluteName)).suffix();
        MPQSettings * settings = MPQSettings::instance();
        QMPQArchive::FileFlags flags = settings->fileFlags(suffix);
        QMPQArchive::CompressionFlags compression = settings->compressionFlags(suffix);
        if (!d->archive->add(d->fileData, d->innerPath, flags, compression)) {
            qDebug() << "QMPQFileEngine::close - can't add";
            return false;
        }
    }

    d->openMode = QIODevice::NotOpen;
    d->fileData.clear();
    d->offset = 0;

    return true;
}

bool QMPQFileEngine::copy(const QString & newName)
{
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
    return QAbstractFileEngine::entryList(filters, filterNames);
}

QAbstractFileEngine::FileFlags QMPQFileEngine::fileFlags(FileFlags type) const
{
    Q_UNUSED(type);
    QAbstractFileEngine::FileFlags result = 0;
    Q_D(const QMPQFileEngine);
    QFileInfo info(d->archiveFilePath);
    if (!info.exists()) {
        return result;
    }
    result |= d->archive->isDir(d->innerPath) ? QAbstractFileEngine::DirectoryType : QAbstractFileEngine::FileType;
    if (d->archive->exists(d->innerPath))
        result |= QAbstractFileEngine::ExistsFlag;
    result |= QAbstractFileEngine::ReadOwnerPerm;
    result |= QAbstractFileEngine::WriteOwnerPerm;
    result |= QAbstractFileEngine::ReadUserPerm;
    result |= QAbstractFileEngine::WriteUserPerm;
    result |= QAbstractFileEngine::ReadGroupPerm;
    result |= QAbstractFileEngine::WriteGroupPerm;
    result |= QAbstractFileEngine::ReadOtherPerm;
    result |= QAbstractFileEngine::WriteOtherPerm;
//    if (d->innerPath == "")
//        result |= QAbstractFileEngine::RootFlag;
    return result;
}

QString QMPQFileEngine::fileName(FileName file) const
{
    Q_D(const QMPQFileEngine);
    if (file == QAbstractFileEngine::DefaultName) {
        return d->fileName;
    }
    if (file == QAbstractFileEngine::BaseName) {
        return d->baseName;
    }
    if (file == QAbstractFileEngine::PathName) {
        qDebug("QMPQFileEngine::fileName");
        if (d->innerPath != "")
            return d->filePath.left(d->filePath.lastIndexOf('/') + 1);
        else
            return d->archiveFilePath.left(d->archiveFilePath.lastIndexOf('/') + 1);
    }
    return d->filePath;
}

QDateTime QMPQFileEngine::fileTime( FileTime time) const
{
    return QAbstractFileEngine::fileTime(time);
}

bool QMPQFileEngine::flush()
{
    return true;
}

bool QMPQFileEngine::isRelativePath() const
{
    return !d_func()->archiveFilePath.contains('/');
}

bool QMPQFileEngine::isCreated()
{
    return d_func()->isCreated;
}

bool QMPQFileEngine::mkdir(const QString & dirName, bool createParentDirectories) const
{
    Q_D(const QMPQFileEngine);
    QString innerPath = dirName.mid(d->archiveFilePath.length() + 1);
    innerPath = innerPath.replace('/', '\\');
    return d->archive->mkdir(innerPath, createParentDirectories);
}

bool QMPQFileEngine::open(QIODevice::OpenMode mode)
{
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

QString QMPQFileEngine::owner(FileOwner /*owner*/) const
{
    return "";
}

uint QMPQFileEngine::ownerId(FileOwner /*owner*/) const
{
    return -2;
}

qint64 QMPQFileEngine::pos() const
{
    return d_func()->offset;
}

bool QMPQFileEngine::seek(qint64 offset)
{
    Q_D(QMPQFileEngine);
    if (d->offset < size())
        d->offset = offset;
    else
        d->offset = size() - 1;
    return true;
}

qint64 QMPQFileEngine::read(char* data, qint64 maxlen)
{
    Q_D(QMPQFileEngine);
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
    Q_D(QMPQFileEngine);
    if (d->innerPath=="") {
        QFile file(d->archiveFilePath);
        d->archive->close();
        return file.remove();
    } else {
        return d->archive->remove(d_func()->innerPath);
    }
}

bool QMPQFileEngine::rename(const QString & newName)
{
    Q_D(const QMPQFileEngine);
    if (d->innerPath=="") {
        QFile file(d->archiveFilePath);
        d->archive->close();
        return file.rename(newName);
    } else {
        QString newInnerPath = newName.mid(d->archiveFilePath.length() + 5);
        newInnerPath = newInnerPath.replace('/', '\\');
        return d->archive->rename(d->innerPath, newInnerPath/*newName.mid(newName.lastIndexOf('/') + 1)*/);
    }
}

bool QMPQFileEngine::rmdir(const QString & dirName, bool /*recurseParentDirectories*/) const
{
    Q_D(const QMPQFileEngine);
    QString realName;

    if (dirName.startsWith("mpq:" + d->archiveFilePath))
        realName = dirName.mid(d->archiveFilePath.length() + 5);
    else
        realName = dirName;

    if (!d->archive->isDir(realName))
        return false;

    return d_func()->archive->remove(realName);
}

void QMPQFileEngine::initArchive()
{
    Q_D(QMPQFileEngine);

    QString archiveFilePath = d->filePath.mid(4);
    archiveFilePath = d->getArchiveFilePath(archiveFilePath);

    if (archiveFilePath == "")
        return;

    d->archive = QMPQArchiveCache::instance()->value(archiveFilePath);

    if (d->archive && d->archive->isOpened()) {
        d->isCreated = true;
    }

    d->archiveFilePath = archiveFilePath;
}

void QMPQFileEngine::setFileName(const QString & fileName)
{
    Q_D(QMPQFileEngine);
    d->fileName = fileName;
    const QString file = QDir::fromNativeSeparators(fileName);
    if (d->filePath == file)
        return;
    QMPQArchiveCache::instance()->remove(d->archiveFilePath);
    d->filePath = file;
    initArchive();
    d->baseName = file.mid(file.lastIndexOf('/') + 1);
    d->innerPath = file.mid(4 + d->archiveFilePath.length() + 1);
    d->innerPath = d->innerPath.replace("/", "\\");

}

bool QMPQFileEngine::setPermissions(uint /*perms*/)
{
    return false;
}

bool QMPQFileEngine::setSize(qint64 /*size*/)
{
    return false;
}

qint64 QMPQFileEngine::size() const
{
    Q_D(const QMPQFileEngine);
    return d->archive->size(d->innerPath);
}

qint64 QMPQFileEngine::write(const char * data, qint64 maxlen)
{
    Q_D(QMPQFileEngine);
    if (!d->openMode & QIODevice::WriteOnly)
        return 0;
    if (maxlen <= 0)
        return 0;
    d->fileData.append(data, maxlen);
    d->offset += maxlen;
    return maxlen;
}
