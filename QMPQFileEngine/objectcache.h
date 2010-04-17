#ifndef OBJECTCACHE_H
#define OBJECTCACHE_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QTimer>
#include <QtCore/QMutex>

class ObjectCache : public QObject
{
    Q_OBJECT

    class ObjectData
    {
    public:
        ObjectData() : object(0), ttl(0), m_ttlIndex(1) {}
        ObjectData(QObject * object, int ttl)
        {
            this->object = object;
            this->ttl = ttl;
            m_ttlIndex = 1;
        }
        QObject * object;
        int ttl;
        void incTtlIndex()
        {
            if (m_ttlIndex < 10)
                m_ttlIndex++;
            ttl = 0;
        }
        int ttlIndex() { return m_ttlIndex; }
    private:
        int m_ttlIndex;
    };

public:
    explicit ObjectCache(QObject *parent = 0);

    Q_INVOKABLE void remove(QVariant key);
    Q_INVOKABLE QObject * value(QVariant key);
    template<class T> T * value(QVariant key) { return qobject_cast<T*>(value(key)); }

protected:
    virtual QObject * create(QVariant key) = 0;

signals:

private slots:
    void timeout();

private:
    QHash<QVariant, ObjectData *> objects;
    QHash<QVariant, ObjectData *> disposedObjects;
    QTimer * timer;
    mutable QMutex mutex;
};

#endif // OBJECTCACHE_H
