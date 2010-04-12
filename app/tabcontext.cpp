#include "tabcontext.h"

#include <ieditor.h>

#include "editormanager.h"
#include "editorview.h"
#include "historymanager.h"

TabContext::TabContext(QObject * parent) :
        QObject(parent),
        m_editor(0)
{
//    m_editor = editor;
    m_widget = new EditorView;
    m_editorManager = new EditorManager(this);
    m_historyManager = new HistoryManager(this);
}

void TabContext::setEditor(IEditor * editor)
{
    if (m_editor != editor) {
        m_widget->setEditor(editor);
        m_editor = editor;
    }
}

