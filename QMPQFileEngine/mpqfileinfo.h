#ifndef MPQFILEINFO_H
#define MPQFILEINFO_H

#include <QExplicitlySharedDataPointer>

class MPQFileInfoData;

class MPQFileInfo
{
    friend class QMPQArchive;

public:
    MPQFileInfo();
    MPQFileInfo(const MPQFileInfo &);
    MPQFileInfo &operator=(const MPQFileInfo &);
    ~MPQFileInfo();

    QString name() const;
    bool isValid() const;
    unsigned localeId() const;
    quint32 fileSize() const;
    quint32 compressedSize() const;
    quint32 blockIndex() const;

private:
    QExplicitlySharedDataPointer<MPQFileInfoData> data;
};

#endif // MPQFILEINFO_H
