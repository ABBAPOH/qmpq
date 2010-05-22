#ifndef QMPQARCHIVE2_P_H
#define QMPQARCHIVE2_P_H

#include "qmpqarchive.h"

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

class QMPQArchivePrivate
{
public:
    /*mutable*/ QMPQArchive::Error error;
    /*mutable*/ QString errorString;
    QString file;
    bool isOpened;
    void * mpq;
    quint32 archiveSize;
    quint32 hashTableSize;
    quint32 blockTableSize;
    quint32 sectorSize;
    quint32 filesCount;

    QMPQArchivePrivate();
};


#endif // QMPQARCHIVE2_P_H
