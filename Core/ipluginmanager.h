#ifndef IPLUGINMANAGER_H
#define IPLUGINMANAGER_H

#include <QObject>

class IPlugin;
class IPluginManager : public QObject
{
    Q_OBJECT

public:
    IPluginManager(QObject * parent = 0) : QObject(parent) {}
    virtual ~IPluginManager() {}

    virtual bool load() = 0;
    virtual QList<IPlugin *> plugins() = 0;

signals:
    void pluginLoaded(IPlugin *);
};

#endif // IPLUGINMANAGER_H
