#ifndef MPQEXTENSIONMANAGER_H
#define MPQEXTENSIONMANAGER_H

#include <QObject>
#include <QStringList>
#include <QHash>

#include "QMPQFileEngine_global.h"

class QMPQFILEENGINESHARED_EXPORT MPQExtensionManager : public QObject
{
Q_OBJECT
public:
    enum CompressionType {
        NONE = 0x00,
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

    enum AddFileOption {
        None = 0x00,
        Implode = 0x01,
        Compress = 0x02,
        Compressed = 0x04,
        Encrypted = 0x08,
        FixKey = 0x10,
        SingleUnit = 0x20,
        DeleteMarker = 0x40,
        SectorCRC = 0x80
    };
    Q_DECLARE_FLAGS(AddFileOptions, AddFileOption)
    Q_FLAGS(AddFileOptions)

    explicit MPQExtensionManager(QObject *parent = 0);
    ~MPQExtensionManager() {}

    void addExtension(const QString & extension, const AddFileOptions & options = None, const CompressionTypes & types = NONE);
    QStringList extensions() { return m_compressionTypes.keys(); }
    void removeExtension(const QString & extension);

    AddFileOptions addFileOptions(const QString & extension) const;
    void setAddFileOptions(const QString & extension, const AddFileOptions & options);

    CompressionTypes compressionTypes(const QString & extension) const;
    void setCompressionTypes(const QString & extension, const CompressionTypes & types);

    static MPQExtensionManager * instance();

signals:

public slots:

private:
    QHash<QString, CompressionTypes> m_compressionTypes;
    QHash<QString, AddFileOptions> m_addFileOptions;
    static MPQExtensionManager * m_instance;
};

#endif // MPQEXTENSIONMANAGER_H
