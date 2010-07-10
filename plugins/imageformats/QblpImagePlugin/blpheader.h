#ifndef BLPHEADER_H
#define BLPHEADER_H

#include <qglobal.h>
#include <QDataStream>

#include <QDebug>

#define BLP1_HEADER_SIZE 156
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

struct JPEGData
{
    quint32 jpegHeaderSize;
    QByteArray jpegHeader;
    QByteArray mipMap[16];
};

struct PalettedData
{

};

void readHeader(QDataStream & s, BLPHeader & head);
void writeHeader(QDataStream & s, const BLPHeader & head);

#endif // BLPHEADER_H
