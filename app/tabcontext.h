#ifndef EDITORCONTEXT_H
#define EDITORCONTEXT_H

#include <QtCore/QObject>

#include "editorview.h"

//#include "editormanager.h"
class EditorManager;
class HistoryManager;
class IEditor;
class QWidget;
class TabContext : public QObject
{
    Q_OBJECT

public:
    TabContext(QObject * parent = 0);
    EditorManager * editorManager() { return m_editorManager; }
    HistoryManager * historyManager() { return m_historyManager; }
    IEditor * editor() { return m_editor; }
    void setEditor(IEditor * editor);
    QWidget * widget() { return m_widget; }

private:
    EditorManager * m_editorManager;
    HistoryManager * m_historyManager;
    IEditor * m_editor;
    EditorView * m_widget;
};

#endif // EDITORCONTEXT_H
