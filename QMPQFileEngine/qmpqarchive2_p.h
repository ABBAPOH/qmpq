#ifndef QMPQARCHIVE2_P_H
#define QMPQARCHIVE2_P_H

#include "qmpqarchive2.h"

class MPQFileInfoData : public QSharedData {
public:
    MPQFileInfoData();

    QString name;
    bool valid;
    unsigned localeId;
    quint32 fileSize;
    quint32 compressedSize;
    quint32 blockIndex;
};

class QMPQArchive2Private
{
public:
    /*mutable*/ QMPQArchive2::Error error;
    /*mutable*/ QString errorString;
    bool isOpened;
    void * mpq;
    quint32 archiveSize;
    quint32 hashTableSize;
    quint32 blockTableSize;
    quint32 sectorSize;
    quint32 filesCount;

    QMPQArchive2Private();
};


#endif // QMPQARCHIVE2_P_H
