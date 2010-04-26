#ifndef QMPQARCHIVECACHE_H
#define QMPQARCHIVECACHE_H

#include "QMPQFileEngine_global.h"

#include "objectcache.h"

class SharedMPQArchive;
class QMPQFILEENGINESHARED_EXPORT QMPQArchiveCache : public ObjectCache
{
Q_OBJECT
public:
    explicit QMPQArchiveCache(QObject *parent = 0);
    virtual QObject * create(QVariant key);
    static void init();
    static QMPQArchiveCache * instance();
    void remove(QString file);
    SharedMPQArchive * value(QString file);

private:
    static QMPQArchiveCache * m_instance;
};

#endif // QMPQARCHIVECACHE_H
