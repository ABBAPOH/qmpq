#include "objectcache.h"

#include <QtCore/QDebug>

uint qHash(QVariant key)
{
    switch(key.type()) {
    case QVariant::Int : return qHash(key.toInt());
    case QVariant::String : return qHash(key.toString());
    default: qWarning() << "can't calculate hash";
        Q_ASSERT(false);
        return 0;
    }
}

ObjectCache::ObjectCache(QObject *parent) :
    QObject(parent), timer(new QTimer(this))
{
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), SLOT(timeout()));
}

void ObjectCache::remove(QVariant key)
{
    mutex.lock();
    ObjectData * data = objects.take(key);
//    data->ttl += 1 << data->ttlIndex();
//    qDebug() << "object will live " << data->ttl;
    data->ttl = 10;
    disposedObjects.insert(key, data);
    mutex.unlock();
}

QObject * ObjectCache::value(QVariant key)
{
    mutex.lock();
    QObject * object = 0;
    ObjectData * data = 0;
    if (!objects.contains(key)) {
        if (disposedObjects.contains(key)) {
            data = disposedObjects.take(key);
            data->ttl = 0;
//            data->incTtlIndex();
//            qDebug() << "ObjectCache::value" << " object " << key << " was restored. TTLIndex now " << data->ttlIndex();
            object = data->object;
        } else {
            object = create(key);
            if (object) {
                data  = new ObjectData();
                data->object = object;
            }
        }
        if (data) {
            objects.insert(key, data);
        }
    }
    object = objects.value(key)->object;
    mutex.unlock();
    return object;
}

void ObjectCache::timeout()
{
    mutex.lock();
    foreach (QVariant key, disposedObjects.keys()) {
        ObjectData * data = disposedObjects.value(key);
//        qDebug() << "removing " << key << ": ttl = " << data->ttl;
        if (!data->ttl--) {
            disposedObjects.remove(key);
            delete data->object;
            delete data;
        }
    }
    mutex.unlock();
}

