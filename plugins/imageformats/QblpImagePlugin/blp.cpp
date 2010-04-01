#include "blp.h"

#include <QImage>
#include <QVariant>
#include <QBuffer>

#include "blpheader.h"
#include "jpeg/qjpeghandler.h"

#include <QDebug>

#include <QFile>

BLPHandler::BLPHandler()
        : quality(75)
{
}

bool BLPHandler::canRead() const
{
    if (canRead(device())) {
        setFormat("blp");
        return true;
    }
    return false;
}

void ARGB2BGRA(QImage & image)
{
    //Converts BGRA to ARGB
    for (int i = 0; i < image.width(); i++)
        for (int j = 0; j < image.height(); j++) {
        int r = qRed(image.pixel(i,j));
        int g = qGreen(image.pixel(i,j));
        int b = qBlue(image.pixel(i,j));
        int a = qAlpha(image.pixel(i,j));
        image.setPixel(i, j, qRgba(b, g, r, a));
    }
}

bool decompressImageJPEG(QByteArray & arr, QImage & image)
{
    QBuffer newDevice(&arr);
    newDevice.open(QBuffer::ReadOnly);
    QJpegHandler h;
    h.setDevice(&newDevice);
    if (!h.canRead(&newDevice)) {
        qWarning() << "not a jpeg";
        return false;
    }
    return h.read(&image);
}

// Applications/Warcraft III/war3.mpq/UI/Console/Human/HumanUITile04.blp
bool BLPHandler::loadJPEG(QDataStream & s, const BLPHeader & blp, QImage &img)
{
    JPEGData data;
    quint32 offset = 0;
    quint32 size = 0;
    quint8 byte;

    s >> data.jpegHeaderSize;
    //  Reads JPEG header
    for (unsigned i = 0; i < data.jpegHeaderSize; i++) {
        s >> byte;
        data.jpegHeader[i] = byte;
    }

    offset = blp.mipMapOffset[0];
    size = blp.mipMapSize[0];

    s.device()->seek(offset);
    //  Reads JPEG data (without header)
    for (unsigned i = 0; i < size; i++) {
        s >> byte;
        data.mipMap[0][i] = byte;
    }

    //  Merges header and data
    data.mipMap[0].prepend(data.jpegHeader);

    if (!decompressImageJPEG(data.mipMap[0], img))
        return false;

    ARGB2BGRA(img);
    return true;
}

bool BLPHandler::loadPalletted( QDataStream & s, const BLPHeader & blp, QImage &img )
{
//    qDebug("BLPHandler::loadPalletted");

    bool hasAplha = blp.flags == 0x8;
    quint32 offset = blp.mipMapOffset[0];
    //quint32 size = blp.mipMapSize[0];

    quint32 palette[256];
    quint8 indexList[blp.width][blp.height];
    quint8 alphaList[blp.width][blp.height];

    for (int i = 0; i < 256; i++) {
        s >> palette[i];
    }
//    qDebug() << blp.pictureType;

    s.device()->seek(offset);

    //        if (blp.pictureType == 3 || blp.pictureType == 4) {
    for (quint32 j = 0; j < blp.height; j++)
        for (quint32 i = 0; i < blp.width; i++) {
        s >> indexList[i][j];
    }
    if (blp.pictureType == 3 || blp.pictureType == 4) {
        for (quint32 j = 0; j < blp.height; j++)
            for (quint32 i = 0; i < blp.width; i++) {
            s >> alphaList[i][j];
        }
    }
    for (quint32 i = 0; i < blp.width; i++) {
        for (quint32 j = 0; j < blp.height; j++) {
            quint8 index = indexList[i][j];
            quint8 red = (palette[index] >> 16) & 0xff;
            quint8 green = (palette[index] >> 8) & 0xff;
            quint8 blue = (palette[index]  >> 0) & 0xff;

            quint8 alpha = 255;
            if (hasAplha) {
                if (blp.pictureType == 3 || blp.pictureType == 4)
                    alpha = alphaList[i][j];
                else if (blp.pictureType == 5) {
                    quint8 index = indexList[i][j];
                    alpha = (palette[index] >> 24) & 0xff;
                }
            }
            img.setPixel(i, j, qRgba(red, green, blue, alpha));
        }
    }
    return true;
}

bool BLPHandler::readData( QDataStream & s, const BLPHeader & blp, QImage &img )
{
    // Create image.
    img = QImage(blp.width, blp.height, QImage::Format_ARGB32);

    if (blp.blpType() == 1) {
        if (blp.type == 0)
            return loadJPEG(s, blp, img);
        else
            return loadPalletted(s, blp, img);
    } else if (blp.blpType() == 2) {
        if (blp.type == 0)
;//            return loadJPEG(s, blp, img);
        else {
            if (blp.encoding == 1) {
                //1 - Uncompressed
                return loadPallettedBLP2(s, blp, img);
//                if (blp.alphaDepth == 0)
                    //0 - uncompressed paletted image with no alpha
//                    return false;
//                else if (blp.alphaDepth == 1)
                    //1 - uncompressed paletted image with 1-bit alpha
//                    return false;
//                else if (blp.alphaDepth == 8)
                    //8 - uncompressed paletted image with 8-bit alpha
//                    return false;
            } else if (blp.encoding == 2) {
                //2 - DirectX compression
                if (blp.alphaDepth == 0)
                    //0 - DXT1 no alpha
                    loadDXT1(s, blp, img);
                else if (blp.alphaDepth == 1)
                    //1 - DXT1 one bit alpha
                    loadDXT1(s, blp, img);
                else if (blp.alphaDepth == 8) {
                    if (blp.alphaEncoding == 1)
                        //1 - DXT3
                        loadDXT3(s, blp, img);
                    else if (blp.alphaEncoding == 7)
                        //7 - DXT5
                        loadDXT5(s, blp, img);
                }
            }
        }
    }

    return true;
}

bool BLPHandler::read(QImage *outImage)
{
    QDataStream s(device());
    s.setByteOrder(QDataStream::LittleEndian);

    // Read image header.
    BLPHeader blp;
    readHeader(s, blp);

    QImage img;
    bool result = readData(s, blp, img);

    if (result == false) {
        qWarning() << "Error loading BLP file.";
        return false;
    }

    *outImage = img;
    return true;

}

void BLPHandler::fillHeader(const QImage &image, BLPHeader & head)
{
    head.BLPType[0] = 'B';
    head.BLPType[1] = 'L';
    head.BLPType[2] = 'P';
    head.width = image.width();
    head.height = image.height();
}

QByteArray compressImageJPEG(const QImage &image, int quality)
{
    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);
    QJpegHandler handler;
    handler.setOption(QImageIOHandler::Quality, quality);
    handler.setDevice(&buffer);
    handler.write(image);
    buffer.close();
    return buffer.data();
}

void cutJPEGHeader(/*BLPHeader & blp, */JPEGData & data)
{
    const int jpegHeaderSize = 4;
    data.jpegHeaderSize = jpegHeaderSize;
    data.jpegHeader = data.mipMap[0].left(jpegHeaderSize);
    for (int i = 0; i < 16; i++) {
        QByteArray bytes = data.mipMap[i];
        if (bytes.isEmpty())
            break;
        data.mipMap[i] = bytes.mid(jpegHeaderSize);
    }
}

void writeJPEGData(QDataStream & s, const JPEGData & data)
{
    s << data.jpegHeaderSize;
    for (quint32 i = 0; i < data.jpegHeaderSize; i++) {
        s << (quint8)data.jpegHeader.at(i);
    }
    //  can't write whole QByteArray because it prepends it's size
    for (quint32 i = 0; i < 16; i++) {
        QByteArray bytes = data.mipMap[i];
        if (bytes.isEmpty())
            break;
        for (int j = 0; j < bytes.size(); j++) {
            s << (quint8)bytes.at(j);
        }
    }
}

void fillOffsets(BLPHeader & blp, JPEGData & data)
{
    for (int i = 0; i < 16; i++) {
        QByteArray bytes = data.mipMap[i];
        if (bytes.isEmpty()) {
            blp.mipMapOffset[i] = 0;
            blp.mipMapSize[i] = 0;
        } else {
            if (i > 0) {
                blp.mipMapOffset[i] = blp.mipMapOffset[i - 1] + blp.mipMapSize[i - 1];
                blp.mipMapSize[i] = bytes.size();
            } else {
                blp.mipMapOffset[i] = BLP1_HEADER_SIZE + 4 + data.jpegHeaderSize; //we add 4 to skip jpegHeaderSize field (32 bits)
                blp.mipMapSize[i] = bytes.size();
            }
        }
    }
}

int calcMipMapsNumber(int width, int height)
{
    long t = width < height ? width : height;
    int numMipmaps = 0;
    while (t % 2 == 0) {
        t /= 2;
        ++numMipmaps;
    }
    return numMipmaps;
}

bool BLPHandler::writeJPEG(const QImage &image)
{
    QImage result = image;
    result = result.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    BLPHeader blp;
    JPEGData data;
    int width = image.width();
    int height = image.height();

    ARGB2BGRA(result); // Changes colors in source image
    fillHeader(result, blp);
    //Writing Header to create JPEG-compressed image
    blp.BLPType[3] = '1';
    blp.type = 0;
//    if (hasAlpha)
        blp.flags = 0x8;
//    else
//        header.flags = 0x0;
    blp.pictureType = 0x5;
    blp.pictureSubType = 0x0;

    int numMipmaps = blp.pictureSubType == 0 ? 1 : calcMipMapsNumber(width, height);
    for (int k = 0; k < numMipmaps; k++) {
        data.mipMap[k] = compressImageJPEG(result, quality);
        result = result.scaled(width /=2, height /= 2);
    }

    cutJPEGHeader(/*blp, */data);
    fillOffsets(blp, data);

    // Write image to device()
    QDataStream s(device());
    s.setByteOrder(QDataStream::LittleEndian);

    writeHeader(s, blp);
    writeJPEGData(s, data);

    return true;
}

bool BLPHandler::write(const QImage &image)
{
    QByteArray format = this->format();
    if (format == "blp")
        format = "blp1jpeg";
    if (format == "blp1jpeg")
        return writeJPEG(image);
    return false;
}

bool BLPHandler::canRead(QIODevice *device)
{
    if (!device) {
        qWarning("BLPHandler::canRead() called with no device");
        return false;
    }
    return device->peek(3) == "BLP";
}

void BLPHandler::setOption(ImageOption option, const QVariant &value)
{
    if (option == Quality)
        quality = value.toInt();
}

bool BLPHandler::supportsOption(ImageOption option) const
{
    return option == Quality;
}
