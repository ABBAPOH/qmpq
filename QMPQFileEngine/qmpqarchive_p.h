#ifndef QMPQARCHIVE2_P_H
#define QMPQARCHIVE2_P_H

#include "qmpqarchive.h"
#include "mpqfileinfo.h"

class MPQFileInfoData : public QSharedData {
public:
    MPQFileInfoData();

    QString name;
    bool valid;
    quint32 hashIndex;
    quint32 codename1;
    quint32 codename2;
    quint32 localeId;
    QLocale locale;
    quint32 blockIndex;
    quint32 fileSize;
    quint32 compressedSize;
    QMPQArchive::FileFlags fileFlags;
    quint64 fileTime;
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
    QLocale locale;
    static QLocale staticLocale;

    QMPQArchivePrivate();
};


#endif // QMPQARCHIVE2_P_H
