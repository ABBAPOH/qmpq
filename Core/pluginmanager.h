#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QtCore/QList>

#include "ipluginmanager.h"
//#include "ieditorfactory.h"

class PluginManager : public IPluginManager
{
    Q_OBJECT

public:
    explicit PluginManager(QObject * parent = 0);
    ~PluginManager();
    bool load();
    QList <IPlugin *> plugins() { return m_plugins; }

private:
    void addPlugin(QObject * object);
    QList <IPlugin *> m_plugins;
};

#endif // PLUGINMANAGER_H
