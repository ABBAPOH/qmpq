#ifndef MPQEXTENSIONMANAGER_H
#define MPQEXTENSIONMANAGER_H

#include <QObject>
#include <QStringList>
#include <QHash>

class MPQExtensionManager : public QObject
{
Q_OBJECT
public:
    enum CompressionType {
        UNKNOWN = 0x00,
        HUFFMAN = 0x01,
        PKWARE = 0x02,
        ZLIB = 0x04,
        BZIP2 = 0x08,
        LZMA = 0x10,
        SPARSE = 0x20,
        ADCPM_Mono = 0x40,
        ADCPM_Stereo = 0x80
    };
    Q_DECLARE_FLAGS(CompressionTypes, CompressionType)
    Q_FLAGS(CompressionTypes)

    explicit MPQExtensionManager(QObject *parent = 0);
    ~MPQExtensionManager() {}
    void addExtension(const QString & extension, const CompressionTypes & types = UNKNOWN);
    CompressionTypes compressionTypes(const QString & extension) const;
    QStringList extensions() { return m_compressionTypes.keys(); }
    void removeExtension(const QString & extension);
    void setCompressionTypes(const QString & extension, const CompressionTypes & types);

    static MPQExtensionManager * instance();

signals:

public slots:

private:
    QHash<QString, CompressionTypes> m_compressionTypes;
    static MPQExtensionManager * m_instance;
};

#endif // MPQEXTENSIONMANAGER_H
