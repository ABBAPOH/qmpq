#ifndef QMPQARCHIVECACHE_H
#define QMPQARCHIVECACHE_H

#include "objectcache.h"

class SharedMPQArchive;
class QMPQArchiveCache : public ObjectCache
{
Q_OBJECT
public:
    explicit QMPQArchiveCache(QObject *parent = 0);
    virtual QObject * create(QVariant key);
    static QMPQArchiveCache * instance();
    void remove(QString file);
    SharedMPQArchive * value(QString file);

private:
    static QMPQArchiveCache * m_instance;

};

#endif // QMPQARCHIVECACHE_H
