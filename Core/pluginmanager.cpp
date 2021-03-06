#include "pluginmanager.h"

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QDebug>

#include "iplugin.h"
//PluginManager * PluginManager::manager = 0;

PluginManager::PluginManager(QObject * parent) :
        IPluginManager(parent)
{
}

PluginManager::~PluginManager()
{
    foreach (IPlugin * plugin, m_plugins) {
        plugin->shutdown();
        delete plugin;
    }
}

//const PluginManager * PluginManager::instance()
//{
//    if (manager)
//        return manager;
//    else
//        return manager = new PluginManager();
//}
void PluginManager::addPlugin(QObject * object)
{
    IPlugin * plugin = qobject_cast<IPlugin *>(object);
    if (plugin) {
       m_plugins.append(plugin);
//       #warning TODO: sort plugin list before initialization
//       plugin->initialize();
   } else {
        qWarning() << "PluginManager::addPlugin" << "not a plugin instance";
   }
}

bool PluginManager::load()
{
    qDebug() << "PluginManager::load";
//    QObject * f = new Test;
//    Q_ASSERT(qobject_cast<IEditorFactory *>(f));

    foreach (QObject *object, QPluginLoader::staticInstances()) {
        addPlugin(object);
     }

    foreach (QString folder, qApp->libraryPaths()) {
        qDebug() << folder;
     QDir pluginsDir = QDir(folder);
     if (!pluginsDir.cd("qmpqplugins"))
         continue;
     qDebug() << pluginsDir.absolutePath();

// #if defined(Q_OS_WIN)
//     if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
//         pluginsDir.cdUp();
// #elif defined(Q_OS_MAC)
//     if (pluginsDir.dirName() == "MacOS") {
//         pluginsDir.cdUp();
//         pluginsDir.cdUp();
//         pluginsDir.cdUp();
//     }
// #endif
//     pluginsDir.cd("plugins");


     foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
         qDebug() << "loading" << fileName;
         QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
         QObject * object = loader.instance();
         if (!object)
             qWarning() << "can't load library";
         addPlugin(object);
     }
 }

     foreach (IPlugin * plugin, plugins()) {
         plugin->initialize();
         emit pluginLoaded(plugin);
     }

     return true;
}


