#include "mpqfileinfo.h"
#include "qmpqarchive_p.h"

#include <QSharedData>

MPQFileInfoData::MPQFileInfoData() :
        valid(false)
{
}

void MPQFileInfoData::setName(const QString & name)
{
    baseName = name.mid(name.lastIndexOf('\\') + 1);
    this->name = name;
}

MPQFileInfo::MPQFileInfo() : data(new MPQFileInfoData)
{
}

MPQFileInfo::MPQFileInfo(const MPQFileInfo &rhs) : data(rhs.data)
{
}

MPQFileInfo &MPQFileInfo::operator=(const MPQFileInfo &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

MPQFileInfo::~MPQFileInfo()
{
}

QString MPQFileInfo::baseName() const
{
    return data->baseName;
}

QString MPQFileInfo::name() const
{
    return data->name;
}

bool MPQFileInfo::isValid() const
{
    return data->valid;
}

quint32 MPQFileInfo::hashIndex() const
{
    return data->hashIndex;
}

quint32 MPQFileInfo::codename1() const
{
    return data->codename1;
}

quint32 MPQFileInfo::codename2() const
{
    return data->codename2;
}

QLocale MPQFileInfo::locale() const
{
    return data->locale;
}

quint32 MPQFileInfo::localeId() const
{
    return data->localeId;
}

quint32 MPQFileInfo::fileSize() const
{
    return data->fileSize;
}

quint32 MPQFileInfo::compressedSize() const
{
    return data->compressedSize;
}

QMPQArchive::FileFlags MPQFileInfo::fileFlags() const
{
    return data->fileFlags;
}

quint32 MPQFileInfo::blockIndex() const
{
    return data->blockIndex;
}
