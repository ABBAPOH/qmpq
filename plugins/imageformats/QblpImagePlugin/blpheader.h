#ifndef BLPHEADER_H
#define BLPHEADER_H

#include <qglobal.h>
#include <QDataStream>

#include <QDebug>

struct BLPHeader
{
    quint8 BLPType[4];                               //BLP1 or BLP2
    quint32 type;  //64                 //0 - Uses JPEG compression
                                        //1 - Uses palettes (uncompressed) or DirectX compression if BLP2
        //BLP1
        quint32 flags;        //96          //#8 - Uses alpha channel (?)

        //BLP2
        quint8       encoding;              //1 - Uncompressed
                                            //2 - DirectX compression
        quint8       alphaDepth;            //0 - No alpha channel
                                            //1 - 1 bit alpha
                                            //8 - 8 bit alpha

        quint8       alphaEncoding;         //0 - DXT1 alpha (0 or 1 bit alpha)
                                            //1 - DXT2/3 alpha (4 bit alpha)
                                            //7 - DXT4/5 alpha (interpolated alpha)
        quint8       hasMips;               //0 - No mip levels
                                            //1 - Mip levels present (the number of levels is determined by the image size)

    quint32 width;
    quint32 height;
    //BLP1
        quint32 pictureType;                    //3 - Uncompressed index list + alpha list
                                                //4 - Uncompressed index list + alpha list
                                                //5 - Uncompressed index list
        quint32 pictureSubType;                 //0 - one mipmap; 1 - all mipmaps present
    quint32 mipMapOffset[16];
    quint32 mipMapSize[16];

    unsigned blpType() const
    {
        switch (BLPType[3]) {
            case '1' : return 1;
            case '2' : return 2;
            default : return 0;
        }
    }
};

static QDataStream & operator>> ( QDataStream & s, BLPHeader & head )
{
    for(int i = 0; i < 4; i++)
        s >> head.BLPType[i];
    s >> head.type;
    if (head.BLPType[3] == '1') {
        s >> head.flags;
        s >> head.width;
        s >> head.height;
        s >> head.pictureType;
        s >> head.pictureSubType;
    } else if (head.BLPType[3] == '2') {
        s >> head.encoding;
        s >> head.alphaDepth;
        s >> head.alphaEncoding;
        s >> head.hasMips;
        s >> head.width;
        s >> head.height;
        qDebug() << head.BLPType[3] << head.type << head.encoding << head.alphaDepth << head.alphaEncoding << head.hasMips;
        qDebug() << head.width << head.height;
    }
    int maxMips = (head.BLPType[3] == '2' && !head.hasMips) ? 1 : 16;
    for (int i = 0; i < maxMips; i++)
        s >> head.mipMapOffset[i];
    for (int i = 0; i < maxMips; i++)
        s >> head.mipMapSize[i];
    return s;
}

static QDataStream & operator<< ( QDataStream & s, BLPHeader & head )
{
    for(int i = 0; i < 4; i++)
        s << head.BLPType[i];
    s << head.type;
    s << head.flags;
    s << head.width;
    s << head.height;
    s << head.pictureType;
    s << head.pictureSubType;
    for (int i = 0; i < 16; i++)
        s << head.mipMapOffset[i];
    for (int i = 0; i < 16; i++)
        s << head.mipMapSize[i];
    return s;
}

#endif // BLPHEADER_H
