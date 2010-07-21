#ifndef ICORE_H
#define ICORE_H

#include <QtCore/QObject>
#include "iactionmanager.h"
#include "icontext.h"
#include "ieditorfactorymanager.h"
#include "iwindowmanager.h"
#include "ipluginmanager.h"
#include "filemanager.h"
#include "coreconstants.h"
#include "ipreferences.h"

//class IActionManager;
//class IContext;
//class IEditorFactoryManager;
class IPlugin;
//class IPluginManager;
//class IFileManager;
class QMainWindow;
class ICore : public QObject
{
    Q_OBJECT

public:
    ICore() {}
    virtual ~ICore() {}

    static ICore * instance();

    virtual bool addObject(QObject * object) = 0;
    virtual QObject * getObject(const QString & name) = 0;

    virtual IActionManager * actionManager() = 0;
    virtual IContext * context() = 0;
    virtual IEditorFactoryManager * editorFactoryManager() = 0;
    virtual FileManager * fileManager() = 0;
    virtual IPluginManager * pluginManager() = 0;
    virtual IWindowManager * windowManager() = 0;

signals:
    void contextAboutToBeChanged(IContext *);
    void contextChanged(IContext *);
};

#endif // ICORE_H
