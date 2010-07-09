#ifndef MPQEXTENSIONMANAGER_H
#define MPQEXTENSIONMANAGER_H

#include <QObject>
#include <QStringList>
#include <QHash>

#include "qmpqarchive.h"
#include "QMPQFileEngine_global.h"

class QMPQFILEENGINESHARED_EXPORT MPQSettings : public QObject
{
Q_OBJECT

public:
    typedef QMPQArchive::FileFlags FileFlags;
    typedef QMPQArchive::CompressionFlags CompressionFlags;

    explicit MPQSettings(QObject *parent = 0);
    ~MPQSettings() {}

    QStringList fileExtensions() const { return m_compressionTypes.keys(); }
    void addFileExtension(const QString & extension, const FileFlags & options = QMPQArchive::None, const CompressionFlags & types = QMPQArchive::NoCompression);
    void removeFileExtension(const QString & extension);

    QStringList MPQExtensions() const { return m_MPQExtensions; }
    void setMPQExtensions(const QStringList & extensions);
    void addMPQExtension(const QString & extension);
    void removeMPQExtension(const QString & extension);

    FileFlags fileFlags(const QString & extension) const;
    void setFileFlags(const QString & extension, const FileFlags & options);

    QMPQArchive::CompressionFlags compressionFlags(const QString & extension) const;
    void setCompressionFlags(const QString & extension, const CompressionFlags & types);

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
