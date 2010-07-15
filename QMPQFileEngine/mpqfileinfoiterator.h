#ifndef MPQFILEINFOITERATOR_H
#define MPQFILEINFOITERATOR_H

#include <QtCore/QStringList>
#include "mpqfileinfo.h"

class QMPQArchive;
class MPQFileInfoIterator
{
    friend class QMPQArchive;

    QMPQArchive * m_archive;
    void * mpq;
    bool m_includeUnknowns;
    quint32 index;
    quint32 filesCount;
    quint32 hashTableSize;
    QList<quint32> indexes;

    void * hFind;
    bool searching;
    MPQFileInfo currentInfo;
    quint32 filesFound;

    MPQFileInfoIterator(QMPQArchive * archive, const QStringList & listfile, bool includeUnknowns);

public:
    ~MPQFileInfoIterator();

    bool hasNext();
    MPQFileInfo next();
};

#endif // MPQFILEINFOITERATOR_H
