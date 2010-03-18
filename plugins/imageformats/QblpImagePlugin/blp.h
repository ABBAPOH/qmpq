#ifndef BLPHANDLER_H
#define BLPHANDLER_H

#include <QImageIOHandler>

class QDataStream;
struct BLPHeader;
class QImage;

class BLPHandler : public QImageIOHandler
{
public:
    BLPHandler();
    bool canRead () const;
    bool read(QImage *image);
    bool write(const QImage &image);

    static bool canRead(QIODevice *device);

//    QVariant option(ImageOption option) const;
    void setOption(ImageOption option, const QVariant &value);
    bool supportsOption(ImageOption option) const;

private:
    bool loadJPEG(QDataStream & s, const BLPHeader & blp, QImage &img);
    bool loadPalletted(QDataStream & s, const BLPHeader & blp, QImage &img);
    bool loadPallettedBLP2(QDataStream & s, const BLPHeader & blp, QImage &img);
    bool loadDXT1(QDataStream & s, const BLPHeader & blp, QImage &img);
    bool loadDXT3(QDataStream & s, const BLPHeader & blp, QImage &img);
    bool loadDXT5(QDataStream & s, const BLPHeader & blp, QImage &img);

    bool readData(QDataStream & s, const BLPHeader & blp, QImage &img);

    void fillHeader(const QImage &image, BLPHeader & head);
    bool writeJPEG(const QImage &image);
    int quality;
};

#endif // BLPHANDLER_H
