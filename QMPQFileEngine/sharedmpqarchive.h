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
};

#endif // SHAREDMPQARCHIVE_H
