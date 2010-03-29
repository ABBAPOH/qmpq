#include "blpheader.h"

void readHeader(QDataStream & s, BLPHeader & head )
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
        qDebug() << (char)head.BLPType[3] << head.type  << head.flags << head.width << head.height << head.pictureType << head.pictureSubType;
    } else if (head.BLPType[3] == '2') {
        s >> head.encoding;
        s >> head.alphaDepth;
        s >> head.alphaEncoding;
        s >> head.hasMips;
        s >> head.width;
        s >> head.height;
        qDebug() << (char)head.BLPType[3] << head.type << head.encoding << head.alphaDepth << head.alphaEncoding << head.hasMips;
        qDebug() << head.width << head.height;
    }
    int maxMips = (head.BLPType[3] == '2' && !head.hasMips) ? 1 : 16;
    qDebug() << "maxMips" << maxMips;
    for (int i = 0; i < maxMips; i++)
        s >> head.mipMapOffset[i];
    for (int i = 0; i < maxMips; i++)
        s >> head.mipMapSize[i];
}

void writeHeader(QDataStream & s, const BLPHeader & head)
{
    for (int i = 0; i < 4; i++)
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
    qDebug() << head.type;
    qDebug() << head.flags;
    qDebug() << head.width;
    qDebug() << head.height;
    qDebug() << head.pictureType;
    qDebug() << head.pictureSubType;
    qDebug() << head.mipMapOffset[0];
    qDebug() << head.mipMapSize[0];
}

