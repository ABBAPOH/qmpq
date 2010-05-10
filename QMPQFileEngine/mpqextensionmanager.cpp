#include "mpqextensionmanager.h"

MPQExtensionManager * MPQExtensionManager::m_instance = 0;

MPQExtensionManager::MPQExtensionManager(QObject *parent) :
    QObject(parent)
{
}

void MPQExtensionManager::addExtension(const QString & extension, const AddFileOptions & options, const CompressionTypes & types)
{
    m_compressionTypes.insert(extension, types);
    m_addFileOptions.insert(extension, options);
}

void MPQExtensionManager::removeExtension(const QString & extension)
{
    m_compressionTypes.remove(extension);
}

MPQExtensionManager::AddFileOptions MPQExtensionManager::addFileOptions(const QString & extension) const
{
    if (m_addFileOptions.contains(extension))
        return m_addFileOptions.value(extension, None);
    else
        return m_addFileOptions.value("*", None);
}

void MPQExtensionManager::setAddFileOptions(const QString & extension, const AddFileOptions & options)
{
    m_addFileOptions.insert(extension, options);
}

MPQExtensionManager::CompressionTypes MPQExtensionManager::compressionTypes(const QString & extension) const
{
    if (m_compressionTypes.contains(extension))
        return m_compressionTypes.value(extension, NONE);
    else
        return m_compressionTypes.value("*", NONE);
}

void MPQExtensionManager::setCompressionTypes(const QString & extension, const CompressionTypes & types)
{
    m_compressionTypes.insert(extension, types);
}

MPQExtensionManager * MPQExtensionManager::instance()
{
    if (m_instance == 0) {
        m_instance = new MPQExtensionManager();
        m_instance->setObjectName("MPQExtensionManager");
    }
    return m_instance;
}
