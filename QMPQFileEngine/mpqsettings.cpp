#include "mpqsettings.h"

MPQSettings * MPQSettings::m_instance = 0;

MPQSettings::MPQSettings(QObject *parent) :
    QObject(parent)
{
}

void MPQSettings::addFileExtension(const QString & extension, const FileFlags & options, const CompressionFlags & types)
{
    m_compressionTypes.insert(extension, types);
    m_addFileOptions.insert(extension, options);
}

void MPQSettings::removeFileExtension(const QString & extension)
{
    m_compressionTypes.remove(extension);
    m_addFileOptions.remove(extension);
}

void MPQSettings::addMPQExtension(const QString & extension)
{
    if (!m_MPQExtensions.contains(extension))
        m_MPQExtensions.append(extension);
}

void MPQSettings::removeMPQExtension(const QString & extension)
{
    m_MPQExtensions.removeAll(extension);
}

MPQSettings::FileFlags MPQSettings::fileFlags(const QString & extension) const
{
    if (m_addFileOptions.contains(extension))
        return m_addFileOptions.value(extension, None);
    else
        return m_addFileOptions.value("*", None);
}

void MPQSettings::setFileFlags(const QString & extension, const FileFlags & options)
{
    m_addFileOptions.insert(extension, options);
}

MPQSettings::CompressionFlags MPQSettings::compressionTypes(const QString & extension) const
{
    if (m_compressionTypes.contains(extension))
        return m_compressionTypes.value(extension, NONE);
    else
        return m_compressionTypes.value("*", NONE);
}

void MPQSettings::setCompressionTypes(const QString & extension, const CompressionFlags & types)
{
    m_compressionTypes.insert(extension, types);
}

MPQSettings * MPQSettings::instance()
{
    if (m_instance == 0) {
        m_instance = new MPQSettings();
        m_instance->setObjectName("MPQExtensionManager");
    }
    return m_instance;
}
