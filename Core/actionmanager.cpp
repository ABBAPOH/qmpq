#include "actionmanager.h"

#include <QtCore/QMetaMethod>
#include <QtGui/QMenu>
#include "icore.h"
#include "ieditor.h"

ActionManager::ActionManager(QObject *parent) :
    IActionManager(parent)
{
    connect(ICore::instance(), SIGNAL(contextChanged(IContext*)), SLOT(updateActions()));
    connect(ICore::instance()->editorFactoryManager(), SIGNAL(currentEditorChanged(IEditor*)), SLOT(updateSave_AsAction()));
}

QMenu * ActionManager::createMenu(const QString & id)
{
    QMenu * menu = new QMenu();
    m_menus.insert(id, menu);
    return menu;
#warning TODO: remove menus when deleted?
}

void ActionManager::registerAction(QAction * action, const QString & id)
{
    m_actions.insert(id, action);
}

QList<QString> ActionManager::menuIds()
{}

QMenu * ActionManager::menu(const QString & id)
{
    return m_menus.value(id);
}

bool hasSlot(QObject * obj, const char * slot)
{
    return obj->metaObject()->indexOfSlot(slot+1) != -1;
}

void ActionManager::undo()
{
    QMetaObject::invokeMethod(ICore::instance()->context()->widget(), "undo");
}

void ActionManager::redo()
{
    QMetaObject::invokeMethod(ICore::instance()->context()->widget(), "redo");
}

void ActionManager::cut()
{
    QMetaObject::invokeMethod(ICore::instance()->context()->widget(), "cut");
}

void ActionManager::copy()
{
    QMetaObject::invokeMethod(ICore::instance()->context()->widget(), "copy");
}

void ActionManager::paste()
{
    QMetaObject::invokeMethod(ICore::instance()->context()->widget(), "paste");
}

void ActionManager::updateActions()
{
    QWidget * widget = ICore::instance()->context()->widget();
    if (!widget)
        return;
    QAction * actionUndo = ICore::instance()->actionManager()->action(Core::ACTION_UNDO);
    Q_ASSERT(actionUndo);
    actionUndo->setEnabled(hasSlot(widget, SLOT(undo())));

    QAction * actionRedo = ICore::instance()->actionManager()->action(Core::ACTION_REDO);
    Q_ASSERT(actionRedo);
    actionRedo->setEnabled(hasSlot(widget, SLOT(redo())));

    QAction * actionCut = ICore::instance()->actionManager()->action(Core::ACTION_CUT);
    Q_ASSERT(actionCut);
    actionCut->setEnabled(hasSlot(widget, SLOT(cut())));

    QAction * actionCopy = ICore::instance()->actionManager()->action(Core::ACTION_COPY);
    Q_ASSERT(actionCopy);
    actionCopy->setEnabled(hasSlot(widget, SLOT(copy())));

    QAction * actionPaste = ICore::instance()->actionManager()->action(Core::ACTION_PASTE);
    Q_ASSERT(actionPaste);
    actionPaste->setEnabled(hasSlot(widget, SLOT(paste())));
}

void ActionManager::updateSave_AsAction()
{
    IEditor * editor = ICore::instance()->editorFactoryManager()->currentEditor();
    QAction * actionSave_As = ICore::instance()->actionManager()->action(Core::ACTION_SAVE_AS);
    QAction * actionSave = ICore::instance()->actionManager()->action(Core::ACTION_SAVE);

    // we disconnect editor in case it already connected
    disconnect(editor, SIGNAL(changed()), this, SLOT(updateSaveAction()));
    connect(editor, SIGNAL(changed()), this, SLOT(updateSaveAction()));

    Q_ASSERT(actionSave_As);
    Q_ASSERT(actionSave);
    if (editor && editor->canSave()) {
        actionSave_As->setEnabled(true);
    } else {
        actionSave_As->setEnabled(false);
    }
    actionSave->setEnabled(false);
}

void ActionManager::updateSaveAction()
{
    IEditor * editor = ICore::instance()->editorFactoryManager()->currentEditor();
    QAction * actionSave = ICore::instance()->actionManager()->action(Core::ACTION_SAVE);
    Q_ASSERT(actionSave);
    if (editor && editor->isModified()) {
        actionSave->setEnabled(true);
    } else {
        actionSave->setEnabled(false);
    }
}

