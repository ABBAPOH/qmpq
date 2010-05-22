#ifndef SHAREDMPQARCHIVE_H
#define SHAREDMPQARCHIVE_H

#include "qmpqarchive.h"
#include "qmpqarchiveex.h"
#include <QHash>
#include <QReadWriteLock>

class SharedMPQArchive : /*public QObject, */public QMPQArchiveEx
{
    Q_OBJECT
public:
    SharedMPQArchive(const QString &file);
    ~SharedMPQArchive();
};

#endif // SHAREDMPQARCHIVE_H
