#include "editormanager.h"

#include "pluginmanager.h"

#include "../plugins/editorplugins/MPQEditor/mpqeditor.h"

EditorManager::EditorManager(QObject *parent) :
        QObject(parent), m_pluginManager(PluginManager::instance())
{
}

IEditor * EditorManager::open(const QString &file)
{
    IEditor * result = 0;
    foreach (IEditor * editor, m_editors) {
        const IEditorFactory * factory = editor->factory();
        if (factory->canHandle(file)) {
            editor->open(file);
            return editor;
        }
    }

    QList<IEditorFactory *> factoryList = m_pluginManager->getSuitableFactories(file);
    if (factoryList.isEmpty())
        return 0;

    IEditorFactory * factory = factoryList.first();
    result = factory->instance();
    if (result) {
        result->open(file);
        m_editors.insert(file, result);

        MPQEditor * MPQeditor = qobject_cast<MPQEditor *>(result->widget());
        if (MPQeditor) {
            connect(MPQeditor, SIGNAL(openRequested(const QString &)), SIGNAL(openRequested(const QString &)));
        }

    }
    return result;
}

IEditor * EditorManager::editor(const QString &file)
{
    return m_editors.value(file);
}

void EditorManager::close(const QString &file)
{
    IEditor * editor = m_editors.value(file);
    m_editors.remove(file);
//    editor->close();
    delete editor;
}




