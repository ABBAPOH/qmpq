#include "objectcache.h"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

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
    connect(qApp, SIGNAL(aboutToQuit()), SLOT(clear()));
}

void ObjectCache::remove(QVariant key)
{
    mutex.lock();
    ObjectData * data = objects.value(key);
    if(data)
//    qDebug() << "ObjectCache::remove" << key << data->refCount - 1;
    if (data && ((data->refCount--) == 0)) {
//        data->ttl += 1 << data->ttlIndex();
//        qDebug() << "object will live " << data->ttl;
        objects.remove(key);
        data->ttl = 10;
        disposedObjects.insert(key, data);
    }
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
            data->refCount = 0;
//            data->incTtlIndex();
//            qDebug() << "ObjectCache::value" << " object " << key << " was restored. TTLIndex now " << data->ttlIndex();
            object = data->object;
        } else {
            object = create(key);
            if (object) {
                emit objectCreated(object);
                data  = new ObjectData();
                data->object = object;
            }
        }
        if (data && object) {
            objects.insert(key, data);
        }
    }
    data = objects.value(key);
    if (data) {
        data->refCount++;
        object = data->object;
//        qDebug() << "ObjectCache::value" << key << data->refCount;
    }
    mutex.unlock();
    return object;
}

void ObjectCache::deleteData(ObjectCache::ObjectData * data)
{
    delete data->object;
    delete data;
}

void ObjectCache::clear()
{
//    qDebug("ObjectCache::clear()");
    mutex.lock();
    foreach (QVariant key, objects.keys()) {
        deleteData(objects.take(key));
    }
    foreach (QVariant key, disposedObjects.keys()) {
        deleteData(disposedObjects.take(key));
    }
    mutex.unlock();
}

void ObjectCache::timeout()
{
    mutex.lock();
    foreach (QVariant key, disposedObjects.keys()) {
        ObjectData * data = disposedObjects.value(key);
//        qDebug() << "removing " << key << ": ttl = " << data->ttl;
        if (!data->ttl--) {
            disposedObjects.remove(key);
            deleteData(data);
        }
    }
    mutex.unlock();
}

