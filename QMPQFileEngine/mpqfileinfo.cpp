#include "mpqfileinfo.h"
#include "qmpqarchive2_p.h"

#include <QSharedData>

//class MPQFileInfoData : public QSharedData {
//public:
//    unsigned m_localeId;
//    quint32 m_fileSize;
//    quint32 m_compressedSize;

//};

MPQFileInfoData::MPQFileInfoData() :
        valid(false)
{
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

QString MPQFileInfo::name() const
{
    return data->name;
}

bool MPQFileInfo::isValid() const
{
    return data->valid;
}

unsigned MPQFileInfo::localeId() const
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

quint32 MPQFileInfo::blockIndex() const
{
    return data->blockIndex;
}
