#include "editorfactorymanager.h"
#include "ieditorfactory.h"

#include <QtGui/QMessageBox>

EditorFactoryManager::EditorFactoryManager(QObject * parent)
    : IEditorFactoryManager(parent), m_editor(0)
{
}

void EditorFactoryManager::addFactory(IEditorFactory * factory)
{
    m_factories.append(factory);
}

bool EditorFactoryManager::close(IEditor * editor)
{
    if (editor->isModified()) {
        int result = ICore::instance()->fileManager()->confirmClose(editor->currentFile());
        if (result == QMessageBox::Save) {
            editor->save(editor->currentFile());
            return true;
        } else if (result == QMessageBox::Cancel) {
            return false;
        }
    }
    editor->close();
    return true;
}

IEditor * EditorFactoryManager::create(const QString & path)
{
    foreach (IEditorFactory * factory, m_factories) {
        if (factory->canHandle(path)) {
            return factory->instance();
        }
    }
    return 0;
}

QIcon EditorFactoryManager::icon(const QString & path)
{
    foreach (IEditorFactory * factory, m_factories) {
        if (factory->canHandle(path)) {
            return factory->icon();
        }
    }
    return QIcon();

}

void EditorFactoryManager::setCurrentEditor(IEditor * editor)
{
    m_editor = editor;
//    updateActions();
    emit currentEditorChanged(m_editor);
}
