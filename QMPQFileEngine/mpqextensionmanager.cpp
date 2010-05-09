#include "mpqextensionmanager.h"

MPQExtensionManager * MPQExtensionManager::m_instance = 0;

MPQExtensionManager::MPQExtensionManager(QObject *parent) :
    QObject(parent)
{
}

void MPQExtensionManager::addExtension(const QString & extension, const CompressionTypes & types)
{
    m_compressionTypes.insert(extension, types);
}

MPQExtensionManager::CompressionTypes MPQExtensionManager::compressionTypes(const QString & extension) const
{
    if (m_compressionTypes.contains(extension))
        return m_compressionTypes.value(extension, UNKNOWN);
    else
        return m_compressionTypes.value("*", UNKNOWN);
}

void MPQExtensionManager::setCompressionTypes(const QString & extension, const CompressionTypes & types)
{
    m_compressionTypes.insert(extension, types);
}

void MPQExtensionManager::removeExtension(const QString & extension)
{
    m_compressionTypes.remove(extension);
}

MPQExtensionManager * MPQExtensionManager::instance()
{
    if (m_instance == 0) {
        m_instance = new MPQExtensionManager();
        m_instance->setObjectName("MPQExtensionManager");
    }
    return m_instance;
}
