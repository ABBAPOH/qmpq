#include "qmpqarchivecache.h"

#include "qmpqarchiveex.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

QMPQArchiveCache * QMPQArchiveCache::m_instance = 0;

QMPQArchiveCache::QMPQArchiveCache(QObject *parent) :
    ObjectCache(parent)
{
}

QObject * QMPQArchiveCache::create(QVariant key)
{
    QMPQArchiveEx * result = new QMPQArchiveEx();
    result->open(key.toString());
    if (result->isOpened()) {
        qDebug() << "QMPQArchiveCache::create" << key;
        return result;
    } else {
        emit objectCreated(result);
        delete result;
        return 0;
    }
}

QMPQArchiveEx * QMPQArchiveCache::value(QString key)
{
    return ObjectCache::value<QMPQArchiveEx>(key);
}

void QMPQArchiveCache::init()
{
    if (m_instance == 0)
        m_instance = new QMPQArchiveCache(qApp);
}

QMPQArchiveCache * QMPQArchiveCache::instance()
{
    return m_instance;
}

void QMPQArchiveCache::remove(QString file)
{
    return ObjectCache::remove(file);
}
