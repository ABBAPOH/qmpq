#include "editormanager.h"

#include <ieditor.h>
#include <icore.h>

#include "../plugins/editorplugins/MPQEditor/mpqeditor.h"

#include <QDebug>

EditorManager::EditorManager(QObject *parent) :
        QObject(parent)/*, m_pluginManager(PluginManager::instance())*/
{
}

IEditor * EditorManager::open(const QString & file)
{
    IEditor * result = 0;
    foreach (IEditor * editor, m_editors) {
        //  we try to find editor that can handle this file
        if (editor->canHandle(file)) {
            result = editor;
        }
    }

    if (!result) {
        //  if no editor was found, we create new one
        IEditorFactoryManager * factoryManager = ICore::instance()->editorFactoryManager();
        result = factoryManager->create(file);
    }

    if (result) {
        //  if we got editor, we open file and save editor to hash
        m_editors.insert(file, result);
        if (file != result->currentFile())
            result->open(file);

//        MPQEditor * MPQeditor = qobject_cast<MPQEditor *>(result->widget());
//        if (MPQeditor) {
//            connect(MPQeditor, SIGNAL(openRequested(const QString &)), SIGNAL(openRequested(const QString &)));
//        }

    }
    return result;
}

IEditor * EditorManager::editor(const QString &file)
{
    return m_editors.value(file);
}

bool EditorManager::close(const QString & file)
{
    IEditor * editor = this->editor(file); // takes only ONE editor
    bool closed = ICore::instance()->editorFactoryManager()->close(editor);

    if (closed)
        m_editors.remove(file);
    else
        return false;

    if (!isOpened(editor)) {
//    editor->close();
        delete editor;
    }
    return true;
}




