#ifndef SHAREDMPQARCHIVE_H
#define SHAREDMPQARCHIVE_H

#include "qmpqarchive.h"
#include <QHash>
#include <QReadWriteLock>

class SharedMPQArchive : public QObject, public QMPQArchive
{
    Q_OBJECT
public:
    SharedMPQArchive(const QString &file);
    ~SharedMPQArchive();

    class MPQsInUse : public QHash<QString, SharedMPQArchive*>
    {
    public:
            mutable QReadWriteLock lock;
    };
    static MPQsInUse& mpqsInUse();
    static SharedMPQArchive* instance(const QString & file);
    static void releaseInstance(const QString & file);

    QAtomicInt refcnt;
private slots:
    void clearCache();
};

#endif // SHAREDMPQARCHIVE_H
