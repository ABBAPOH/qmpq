#include "qmpqarchivecache.h"

#include "sharedmpqarchive.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

QMPQArchiveCache * QMPQArchiveCache::m_instance = 0;

QMPQArchiveCache::QMPQArchiveCache(QObject *parent) :
    ObjectCache(parent)
{
}

QObject * QMPQArchiveCache::create(QVariant key)
{
    SharedMPQArchive * result = new SharedMPQArchive(key.toString());
    if (result->isOpened()) {
        qDebug() << "QMPQArchiveCache::create" << key;
        return result;
    } else {
        delete result;
        return 0;
    }
}

SharedMPQArchive * QMPQArchiveCache::value(QString key)
{
    return ObjectCache::value<SharedMPQArchive>(key);
}

QMPQArchiveCache * QMPQArchiveCache::instance()
{
    if (m_instance == 0)
        m_instance = new QMPQArchiveCache(qApp);
    return m_instance;
}

void QMPQArchiveCache::remove(QString file)
{
    return ObjectCache::remove(file);
}
