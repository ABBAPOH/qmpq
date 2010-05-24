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
    QStringList m_listfile;
    bool m_includeUnknowns;
    quint32 index;
    quint32 filesFound;
    int listfileIndex;
    quint32 hashIndex;
    quint32 filesCount;
    quint32 hashTableSize;
    MPQFileInfo nextInfo;
    QList<quint32> indexes;

    MPQFileInfoIterator(QMPQArchive * archive, const QStringList & listfile, bool includeUnknowns);

    MPQFileInfo getNext();
public:
//    MPQFileInfoIterator();

    bool hasNext();
    MPQFileInfo next();
};

#endif // MPQFILEINFOITERATOR_H
