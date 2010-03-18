#include "blp.h"

#include <QImageIOPlugin>

class BLPPlugin : public QImageIOPlugin
{
public:
        QStringList keys() const;
        Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
        QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const;
};

QStringList BLPPlugin::keys() const
{
        return QStringList() << "blp" << "BLP";
}

QImageIOPlugin::Capabilities BLPPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
        if (format == "blp" || format == "blp1jpeg")
                return Capabilities(CanRead | CanWrite);
        if (!format.isEmpty())
                return 0;
        if (!device->isOpen())
                return 0;

        Capabilities cap;
        if (device->isReadable() && BLPHandler::canRead(device))
                cap |= CanRead;
        if (device->isWritable())
                cap |= CanWrite;
        return cap;
}

QImageIOHandler *BLPPlugin::create(QIODevice *device, const QByteArray &format) const
{
        QImageIOHandler *handler = new BLPHandler;
        handler->setDevice(device);
        handler->setFormat(format);
        return handler;
}

Q_EXPORT_STATIC_PLUGIN(BLPPlugin)
Q_EXPORT_PLUGIN2(blp, BLPPlugin)
