#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QtCore/QList>

#include "ieditorfactory.h"

class PluginManager
{
    PluginManager();
    ~PluginManager();
public:
    static const PluginManager * instance();
    void loadPlugins();
    QList <IEditorFactory *> getSuitableFactories(const QString &file) const;
    IEditor * getEditor(const QString &file, QWidget * parent = 0) const;
    static void shutdown();
//    void releaseEditor() {}
private:
    QList <IEditorFactory *> m_factories;
    static PluginManager * manager;
};

#endif // PLUGINMANAGER_H
