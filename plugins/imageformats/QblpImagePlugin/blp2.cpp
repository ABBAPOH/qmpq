#include "blpheader.h"
#include "blp.h"

#include <QImage>
#include <QDebug>

void decodeColor(quint16 color, quint8 & red, quint8 & green, quint8 & blue)
{
    red = ((color >> 11) & 0x1f) << 3;
    green = ((color >> 5) & 0x3f) << 2;
    blue = (color & 0x1f) << 3;
}

quint8 calcC2(quint8 c0, quint8 c1)
{
    return 2*c0/3 + c1/3;
}

quint8 calcC2a(quint8 c0, quint8 c1)
{
    return c0/2 + c1/2;
}

quint8 calcC3(quint8 c0, quint8 c1)
{
    return c0/3 + 2*c1/3;
}

QRgb qRgba(QRgb rgb, int a)
{
    return qRgba(qRed(rgb), qGreen(rgb), qBlue(rgb), a);
}

QRgb * DXTFillColors(quint16 c0, quint16 c1, quint32 table, bool dxt1a = false)
{
    QRgb * result = new QRgb[16];
    quint8 r[4];
    quint8 g[4];
    quint8 b[4];
    quint8 a[4];
    for (int i = 0; i < 4; i++)
        a[i] = 255;
    decodeColor(c0, r[0], g[0], b[0]);
    decodeColor(c1, r[1], g[1], b[1]);
    if (!dxt1a) {
        r[2] = calcC2(r[0], r[1]);
        g[2] = calcC2(g[0], g[1]);
        b[2] = calcC2(b[0], b[1]);
        r[3] = calcC3(r[0], r[1]);
        g[3] = calcC3(g[0], g[1]);
        b[3] = calcC3(b[0], b[1]);
    } else {
        r[2] = calcC2a(r[0], r[1]);
        g[2] = calcC2a(g[0], g[1]);
        b[2] = calcC2a(b[0], b[1]);
        r[3] = 0;
        g[3] = 0;
        b[3] = 0;
        a[3] = 0;
    }

    for (int k = 0; k < 4; k++)
        for (int l = 0; l < 4; l++) {
        unsigned index = table & 0x0003;
        table = table >> 2;

        int red = r[index];
        int green = g[index];
        int blue = b[index];
        int alpha = a[index];
        result[k*4+l] = qRgba(red, green, blue, alpha);
        //                    img.setPixel(j*4+l, i*4+k, qRgb(red, green, blue));
    }
    return result;
}

bool BLPHandler::loadPallettedBLP2( QDataStream & s, const BLPHeader & blp, QImage &img )
{
    qDebug() << "BLPHandler::loadDXT1";

    quint32 palette[256];
    quint8 indexList[blp.width][blp.height];
    quint8 alphaList[blp.width][blp.height];
    bool hasAplha = false;

    for (int i = 0; i < 256; i++)
        s >> palette[i];

    quint32 offset = blp.mipMapOffset[0];
    s.device()->seek(offset);

    for (quint32 i = 0; i < blp.width; i++)
        for (quint32 j = 0; j < blp.height; j++)
            s >> indexList[i][j];

    if (blp.alphaDepth == 1) {
        hasAplha = true;
        int count = 0;
        quint8 temp;
        for (quint32 i = 0; i < blp.width; i++)
            for (quint32 j = 0; j < blp.height; j++) {
                if (count == 0)
                    s >> temp;
                alphaList[i][j] = temp & 0x8;
                temp >>= 1;
                ++count %= 8;
//                count = (count+1)%8;
        }

    } else if (blp.alphaDepth == 8) {
        hasAplha = true;
        for (quint32 i = 0; i < blp.width; i++)
            for (quint32 j = 0; j < blp.height; j++)
                s >> alphaList[i][j];
    }

    for (quint32 i = 0; i < blp.width; i++)
        for (quint32 j = 0; j < blp.height; j++) {
            quint8 red = (palette[indexList[i][j]] >> 16) & 0xff;
            quint8 green = (palette[indexList[i][j]] >> 8) & 0xff;
            quint8 blue = (palette[indexList[i][j]]  >> 0) & 0xff;
            quint8 alpha = hasAplha ? alphaList[i][j] : 255;
            img.setPixel(j, i, qRgba(red, green, blue, alpha));
    }
    return true;
}

bool BLPHandler::loadDXT1( QDataStream & s, const BLPHeader & blp, QImage &img )
{
    qDebug() << "BLPHandler::loadDXT1";

    quint32 offset = blp.mipMapOffset[0];
    s.device()->seek(offset);

    for (quint32 i = 0; i < blp.height/4; i++)
        for (quint32 j = 0; j < blp.width/4; j++) {
            quint16 c0, c1;  // color
            quint32 table; // indexes for colors
            s >> c0;
            s >> c1;
            s >> table;
            QRgb * arr = 0;
            if (c0 > c1)
                arr = DXTFillColors(c0, c1, table, false);
            else
                arr = DXTFillColors(c0, c1, table, true);

//            quint8 r[4];
//            quint8 g[4];
//            quint8 b[4];
//            decodeColor(c[0], r[0], g[0], b[0]);
//            decodeColor(c[1], r[1], g[1], b[1]);
//            if (c[0] > c[1]) {
//                r[2] = calcC2(r[0], r[1]);
//                g[2] = calcC2(g[0], g[1]);
//                b[2] = calcC2(b[0], b[1]);
//                r[3] = calcC3(r[0], r[1]);
//                g[3] = calcC3(g[0], g[1]);
//                b[3] = calcC3(b[0], b[1]);
//            } else {
//                //TODO: write
//            }

            for (int k = 0; k < 4; k++)
                for (int l = 0; l < 4; l++) {
//                    unsigned index = table & 0x0003;
//                    table = table >> 2;
//
//                    int red = r[index];
//                    int green = g[index];
//                    int blue = b[index];

                    img.setPixel(j*4+l, i*4+k, arr[k*4+l]);
//                    img.setPixel(j*4+l, i*4+k, qRgb(red, green, blue));
                }
            delete arr;
        }
    return true;
}

void setAplphaDXT3(QRgb * rgbArr, quint64 alphaList)
{
    const static quint8 alphas[] = {0, 16, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
    for (int i = 0; i < 16; i++) {
        quint8 alpha = alphaList &0x0f;
//        quint8 alpha = alphaList & 0xf000000000000000;
//        quint8 alpha = alphaList >> 60;
//        alphaList <<= 4;
        alphaList >>= 4;
        rgbArr[i] = qRgba(rgbArr[i], alphas[alpha]);
    }
}

bool BLPHandler::loadDXT3( QDataStream & s, const BLPHeader & blp, QImage &img )
{
    qDebug() << "BLPHandler::loadDXT3";

    quint32 offset = blp.mipMapOffset[0];
    qDebug() << offset;
    qDebug() << blp.mipMapSize[0];
    s.device()->seek(offset);
    for (quint32 i = 0; i < blp.height/4; i++)
        for (quint32 j = 0; j < blp.width/4; j++) {
            quint64 alpha; // together 128bit !!! calculate alpha!!!
            quint16 c0, c1;  // color
            quint32 table; // indexes for colors
            s >> alpha;
            s >> c0;
            s >> c1;
            s >> table;

            QRgb * arr = DXTFillColors(c0, c1, table);
            setAplphaDXT3(arr, alpha);

//            quint8 r[4];
//            quint8 g[4];
//            quint8 b[4];
//            decodeColor(c[0], r[0], g[0], b[0]);
//            decodeColor(c[1], r[1], g[1], b[1]);
//            r[2] = calcC2(r[0], r[1]);
//            g[2] = calcC2(g[0], g[1]);
//            b[2] = calcC2(b[0], b[1]);
//            r[3] = calcC3(r[0], r[1]);
//            g[3] = calcC3(g[0], g[1]);
//            b[3] = calcC3(b[0], b[1]);

            for (int k = 0; k < 4; k++)
                for (int l = 0; l < 4; l++) {
//                    unsigned index = table & 0x0003;
//                    table = table >> 2;

//                    int red = r[index];
//                    int green = g[index];
//                    int blue = b[index];
//                    img.setPixel(j*4+l, i*4+k, qRgb(red, green, blue));
                img.setPixel(j*4+l, i*4+k, arr[k*4+l]);

                }
            delete arr;
        }
    return true;
}

void setAplphaDXT5(QRgb * rgbArr, quint64 alphas)
{
    qDebug() << alphas;
    quint8 a[8];
//    a[0] = alphas >> 56;
//    a[1] = (alphas >> 48) & 0x00ff;
    a[0] = alphas & 0xff;
    a[1] = (alphas >> 8) & 0xff;
    if (a[0] > a[1]) {
        a[2] = (6*a[0] + 1*a[1])/7;
        a[3] = (5*a[0] + 2*a[1])/7;
        a[4] = (4*a[0] + 3*a[1])/7;
        a[5] = (3*a[0] + 4*a[1])/7;
        a[6] = (2*a[0] + 5*a[1])/7;
        a[7] = (1*a[0] + 6*a[1])/7;
    } else {
        a[2] = (4*a[0] + 1*a[1])/5;
        a[3] = (3*a[0] + 2*a[1])/5;
        a[4] = (2*a[0] + 3*a[1])/5;
        a[5] = (1*a[0] + 4*a[1])/5;
        a[6] = 0;
        a[7] = 255;
    }
//    alphas = alphas << 16;
    alphas >>= 16;
    for (int i = 0; i < 16; i++) {
//    for (int k = 0; k < 4; k++)
//        for (int l = 0; l < 4; l++) {
//        quint8 index = alphas & 0xe000000000000000;
        quint8 index = alphas & 0x07;
//        quint8 index = alphas >> 61;
//        qDebug() << a[index];
        rgbArr[i] = qRgba(rgbArr[i], a[index]);
//        rgbArr[l*4+k] = qRgba(rgbArr[l*4+k], a[index]);
        alphas = alphas >> 3;
    }
}

bool BLPHandler::loadDXT5( QDataStream & s, const BLPHeader & blp, QImage &img )
{
    qDebug() << "BLPHandler::loadDXT5";

    quint32 offset = blp.mipMapOffset[0];
//    qDebug() << offset;
//    qDebug() << blp.mipMapSize[0];
    s.device()->seek(offset);
    for (quint32 i = 0; i < blp.height/4; i++)
        for (quint32 j = 0; j < blp.width/4; j++) {
            quint64 alpha; // together 128bit !!! calculate alpha!!!
            quint16 c0, c1;  // color
            quint32 table; // indexes for colors
//            quint32 a1, a2;
//            s >> a2; s >> a1;
//            alpha = ((quint64)a1 << 32) + a2;
            s >> alpha;
            s >> c0;
            s >> c1;
            s >> table;

            QRgb * arr = DXTFillColors(c0, c1, table);
            setAplphaDXT5(arr, alpha);

            for (int k = 0; k < 4; k++)
                for (int l = 0; l < 4; l++) {
                   img.setPixel(j*4+l, i*4+k, arr[k*4+l]);
            }
            delete arr;
        }
    return true;
}
