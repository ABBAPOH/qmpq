#include "editorview.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBar>
#include <ieditor.h>

EditorView::EditorView(QWidget *parent) :
    QWidget(parent)
{
    QWidget * widget = new QWidget();
    layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);
}

void EditorView::setEditor(IEditor * editor)
{
    m_editor = editor;
    QLayoutItem * child = 0;
    while ((child = layout->takeAt(0)) != 0) {
        child->widget()->hide();
    }
    layout->addWidget(m_editor->toolBar());
    layout->addWidget(m_editor->widget());
    m_editor->toolBar()->show();
    m_editor->widget()->show();
    m_editor->toolBar()->setMaximumHeight(24);
}

