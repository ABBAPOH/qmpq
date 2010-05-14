#ifndef MPQEXTENSIONMANAGER_H
#define MPQEXTENSIONMANAGER_H

#include <QObject>
#include <QStringList>
#include <QHash>

#include "QMPQFileEngine_global.h"

class QMPQFILEENGINESHARED_EXPORT MPQSettings : public QObject
{
Q_OBJECT
public:
    enum CompressionFlag {
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
    Q_DECLARE_FLAGS(CompressionFlags, CompressionFlag)
    Q_FLAGS(CompressionFlags)

    enum FileFlag {
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
    Q_DECLARE_FLAGS(FileFlags, FileFlag)
    Q_FLAGS(FileFlags)

    explicit MPQSettings(QObject *parent = 0);
    ~MPQSettings() {}

    QStringList fileExtensions() const { return m_compressionTypes.keys(); }
    void addFileExtension(const QString & extension, const FileFlags & options = None, const CompressionFlags & types = NONE);
    void removeFileExtension(const QString & extension);

    QStringList MPQExtensions() const { return m_MPQExtensions; }
    void setMPQExtensions(const QStringList & extensions) { m_MPQExtensions = extensions; }
    void addMPQExtension(const QString & extension);
    void removeMPQExtension(const QString & extension);

    FileFlags fileFlags(const QString & extension) const;
    void setFileFlags(const QString & extension, const FileFlags & options);

    CompressionFlags compressionTypes(const QString & extension) const;
    void setCompressionTypes(const QString & extension, const CompressionFlags & types);

    static MPQSettings * instance();

signals:

public slots:

private:
    QHash<QString, CompressionFlags> m_compressionTypes;
    QHash<QString, FileFlags> m_addFileOptions;
    QStringList m_MPQExtensions;
    static MPQSettings * m_instance;
};

#endif // MPQEXTENSIONMANAGER_H
