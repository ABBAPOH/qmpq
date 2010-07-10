#include "dds.h"
#include "ddsheader.h"
#include "qdxt.h"

#include <QtGui/QImage>
#include <QtCore/QDataStream>

#include <QDebug>

static QDataStream & operator>>(QDataStream &s, DDSPixelFormat &pixelFormat)
{
    s >> pixelFormat.size;
    s >> pixelFormat.flags;
    s >> pixelFormat.fourCC;
    s >> pixelFormat.rgbBitCount;
    s >> pixelFormat.rBitMask;
    s >> pixelFormat.gBitMask;
    s >> pixelFormat.bBitMask;
    s >> pixelFormat.aBitMask;
    return s;
}

static QDataStream & operator>>(QDataStream &s, DDSHeader &header)
{
    s >> header.size;
    s >> header.flags;
    s >> header.height;
    s >> header.width;
    s >> header.linearSize;
    s >> header.depth;
    s >> header.mipMapCount;
    for (int i = 0; i< 11; i++) {
        s >> header.reserved1[i];
    }
    s >> header.ddsPixelFormat;
    s >> header.caps;
    s >> header.caps2;
    s >> header.caps3;
    s >> header.caps4;
    s >> header.reserved2;
    return s;
}

DDSHandler::DDSHandler()
{
}

bool DDSHandler::canRead() const
{
    if (canRead(device())) {
        setFormat("dds");
        return true;
    }
    return false;
}

int shift(quint32 mask)
{
    int result = 32;
    while (mask != 0) {
        result--;
        mask <<= 1;
    }
    return result;
}

bool readData(QDataStream & s, const DDSHeader & dds, QImage &img)
{
    quint32 flags = dds.ddsPixelFormat.flags;
    if (flags & DDSPixelFormat::DDPF_FOURCC) {
        if (memcmp(&dds.ddsPixelFormat.fourCC, "DXT1", 4) == 0)
            img = QDXT::loadDXT1(s, dds.width, dds.height);

        if (memcmp(&dds.ddsPixelFormat.fourCC, "DXT5", 4) == 0)
            img = QDXT::loadDXT5(s, dds.width, dds.height);
    }

    quint32 rBitMask = dds.ddsPixelFormat.rBitMask;
    quint32 gBitMask = dds.ddsPixelFormat.gBitMask;
    quint32 bBitMask = dds.ddsPixelFormat.bBitMask;
    int redShift = shift(rBitMask);
    int greenShift = shift(gBitMask);
    int blueShift = shift(bBitMask);
    quint32 aBitMask = 0;
    int alphaShift = 0;
    if (dds.ddsPixelFormat.flags & DDSPixelFormat::DDPF_ALPHAPIXELS) {
        dds.ddsPixelFormat.aBitMask;
        alphaShift = shift(aBitMask);
    }
//    qDebug() << "shifts" << redShift << greenShift << blueShift << alphaShift;
    if (flags & DDSPixelFormat::DDPF_RGB) {
        qDebug("read rgb image");
        if (dds.ddsPixelFormat.rgbBitCount != 32) {
            qWarning() << "can't read image with bit count " << dds.ddsPixelFormat.rgbBitCount;
            return false;
        }
        if (dds.ddsPixelFormat.flags & DDSPixelFormat::DDPF_ALPHAPIXELS) {
            img = QImage(dds.width, dds.height, QImage::Format_ARGB32);
        } else {
            img = QImage(dds.width, dds.height, QImage::Format_RGB32);
        }
        for (int i = 0; i < dds.width; i++)
            for (int j = 0; j < dds.height; j++) {
                quint32 color;
                s >> color;
                int red = (color & rBitMask) >> redShift;
                int green = (color & gBitMask) >> greenShift;
                int blue = (color & bBitMask) >> blueShift;
                int alpha = (color & aBitMask) >> alphaShift;
//                qDebug() << red<< green<< blue<< alpha;
                img.setPixel(j, i, qRgba(red, green, blue, 255 - alpha));
            }
    }
//    img = QImage(dds.width, dds.height, QImage::Format_ARGB32);
//    return loadDXT5(s, dds, img);
    return true;
}

bool DDSHandler::read(QImage *outImage)
{
    qDebug() << "DDSHandler::read";
    QDataStream s(device());
    s.setByteOrder(QDataStream::LittleEndian);

    // Read image header.
    DDSHeader dds;
    quint32 magic;
    s >> magic;
    s >> dds;

    QImage img;
    bool result = readData(s, dds, img);

    if (result == false) {
        qWarning() << "Error loading dds file.";
        return false;
    }

    *outImage = img;
    return true;
}

bool DDSHandler::write(const QImage &outImage)
{
}

QByteArray DDSHandler::name() const
{
    return "dds";
}

bool DDSHandler::canRead(QIODevice *device)
{
    if (!device) {
        qWarning("DDSHandler::canRead() called with no device");
        return false;
    }

    return device->peek(4) == "DDS ";
}


// ===================== DDSPlugin =====================

class DDSPlugin : public QImageIOPlugin
{
public:
	QStringList keys() const;
	Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
	QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const;
};

QStringList DDSPlugin::keys() const
{
    return QStringList() << "dds" << "DDS";
}

QImageIOPlugin::Capabilities DDSPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "dds" || format == "DDS")
		return Capabilities(CanRead | CanWrite);
	if (!format.isEmpty())
		return 0;
	if (!device->isOpen())
		return 0;
	
	Capabilities cap;
    if (device->isReadable() && DDSHandler::canRead(device))
		cap |= CanRead;
	if (device->isWritable())
		cap |= CanWrite;
	return cap;
}

QImageIOHandler *DDSPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *handler = new DDSHandler;
	handler->setDevice(device);
	handler->setFormat(format);
	return handler;
}

Q_EXPORT_STATIC_PLUGIN(DDSPlugin)
Q_EXPORT_PLUGIN2(dds, DDSPlugin)
