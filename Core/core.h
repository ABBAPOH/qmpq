#ifndef CORE_H
#define CORE_H

#include "icore.h"

class EditorFactoryManager;
class CoreImpl : public ICore
{
Q_OBJECT
public:
    explicit CoreImpl();
    ~CoreImpl();

    bool addObject(QObject * object);
    QObject * getObject(const QString & name);

    IActionManager * actionManager();
    IContext * context();
    IEditorFactoryManager * editorFactoryManager();
    FileManager * fileManager();
    IPluginManager * pluginManager();
    IWindowManager * windowManager();

signals:

public slots:
    void onFocusChange(QWidget *, QWidget *);

private:
    IActionManager * m_actionManager;
    IContext * m_context;
    EditorFactoryManager * m_editorFactoryManager;
    FileManager * m_fileManager;
    IWindowManager * m_windowManager;
    IPluginManager * m_pluginManager;
};

#endif // CORE_H
