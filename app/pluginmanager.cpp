#include "pluginmanager.h"

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtGui/QApplication>

PluginManager * PluginManager::manager = 0;

PluginManager::PluginManager()
{
    loadPlugins();
}

PluginManager::~PluginManager()
{
    foreach (IEditorFactory * factory, m_factories) {
        factory->shutdown();
        delete factory;
    }
}

const PluginManager * PluginManager::instance()
{
    if (manager)
        return manager;
    else
        return manager = new PluginManager();
}

void PluginManager::loadPlugins()
{
//    QObject * f = new Test;
//    Q_ASSERT(qobject_cast<IEditorFactory *>(f));

    foreach (QObject *plugin, QPluginLoader::staticInstances()) {
         IEditorFactory * factory = qobject_cast<IEditorFactory *>(plugin);
         if (factory)
            m_factories.append(factory);
     }

     QDir pluginsDir = QDir(qApp->applicationDirPath());

 #if defined(Q_OS_WIN)
     if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
         pluginsDir.cdUp();
 #elif defined(Q_OS_MAC)
     if (pluginsDir.dirName() == "MacOS") {
         pluginsDir.cdUp();
         pluginsDir.cdUp();
         pluginsDir.cdUp();
     }
 #endif
     pluginsDir.cd("plugins");

     foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
         QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
         QObject *plugin = loader.instance();
         IEditorFactory * factory = qobject_cast<IEditorFactory *>(plugin);
         if (factory) {
            m_factories.append(factory);
        }
     }
}

QList <IEditorFactory *> PluginManager::getSuitableFactories(const QString &file) const
{
    QList <IEditorFactory *> result;
    foreach (IEditorFactory * factory, m_factories)
        if (factory->canHandle(file))
            result.append(factory);
    return result;
}

IEditor * PluginManager::getEditor(const QString &file, QWidget * parent) const
{
    foreach (IEditorFactory * factory, m_factories)
        if (factory->canHandle(file))
            return factory->instance(parent);
    return 0;
}

void PluginManager::shutdown()
{
//    qDebug("PluginManager::shutdown");
    delete manager;
    manager = 0;
}


