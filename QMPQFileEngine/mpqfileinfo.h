#ifndef MPQFILEINFO_H
#define MPQFILEINFO_H

#include <QExplicitlySharedDataPointer>

#include "QMPQFileEngine_global.h"

#include "qmpqarchive.h"

class MPQFileInfoData;
//class QMPQArchive;//{
//class QMPQArchive::FileFlags;
//class FileFlags;
//}

class QMPQFILEENGINESHARED_EXPORT MPQFileInfo
{
    friend class QMPQArchive;
    friend class MPQFileInfoIterator;

public:
    MPQFileInfo();
    MPQFileInfo(const MPQFileInfo &);
    MPQFileInfo &operator=(const MPQFileInfo &);
    ~MPQFileInfo();

    QString name() const;
    bool isValid() const;
    quint32 hashIndex() const;
    quint32 codename1() const;
    quint32 codename2() const;
    QLocale locale() const;
    quint32 localeId() const;
    quint32 blockIndex() const;
    quint32 fileSize() const;
    quint32 compressedSize() const;
    QMPQArchive::FileFlags fileFlags() const;

private:
    QExplicitlySharedDataPointer<MPQFileInfoData> data;
};

#endif // MPQFILEINFO_H
