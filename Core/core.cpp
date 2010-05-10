#include "core.h"

#include "actionmanager.h"
#include "basecontext.h"
#include "editorfactorymanager.h"
#include "ieditorfactory.h"
//#include "iwindowmanager.h"
#include "iplugin.h"
#include "pluginmanager.h"
#include "preferencesmanager.h"

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>

static ICore * m_instance = 0;

ICore* ICore::instance()
{
    return m_instance;
}

Core::Core()
    : ICore()
{
    m_instance = this;
    m_context = new BaseContext;
    m_editorFactoryManager = new EditorFactoryManager(this);
    m_fileManager = new FileManager(this);
    m_pluginManager = new PluginManager(this);

    m_actionManager = new ActionManager(this); // must be initialized after EditorFactoryManager
    m_windowManager = 0;

    PreferencesManager * preferenecesManager = new PreferencesManager;
    preferenecesManager->setObjectName("PreferencesManager");
    addObject(preferenecesManager);

    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)), SLOT(onFocusChange(QWidget *, QWidget *)));
}

Core::~Core()
{
}

bool Core::addObject(QObject * object)
{
    if (!object)
        return false;
    IWindowManager * fileManager = qobject_cast<IWindowManager *>(object);
    if (fileManager) {
        m_windowManager = fileManager;
        return true;
    }

    IEditorFactory * factory = qobject_cast<IEditorFactory *>(object);
    if (factory) {
        m_editorFactoryManager->addFactory(factory);
        return true;
    }

    object->setParent(this);

    return true;
}

QObject * Core::getObject(const QString & name)
{
    return findChild<QObject *>(name);
}

IActionManager * Core::actionManager()
{
    return m_actionManager;
}

IContext * Core::context()
{
    return m_context;
}

IEditorFactoryManager * Core::editorFactoryManager()
{
    return m_editorFactoryManager;
}

IWindowManager * Core::windowManager()
{
    return m_windowManager;
}

FileManager * Core::fileManager()
{
    return m_fileManager;
}

IPluginManager * Core::pluginManager()
{
    return m_pluginManager;
}

void Core::onFocusChange(QWidget *, QWidget *newWidget)
{
    emit contextAboutToBeChanged(m_context);
    delete m_context;
    QMainWindow * mainWindow = qobject_cast<QMainWindow *>(qApp->activeWindow());
    m_context = new BaseContext(newWidget, mainWindow);
    emit contextChanged(m_context);
}
