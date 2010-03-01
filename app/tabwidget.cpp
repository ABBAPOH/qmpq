#include "tabwidget.h"

#include "editorview.h"

#include <QtGui/QDesktopServices>

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent),
    m_currentWidget(0),
    m_previousWidget(0)
{
    connect(this, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));
    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
}

QWidget * TabWidget::previousWidget()
{
    return m_previousWidget;
}

void TabWidget::newTab()
{
    EditorView * view = new EditorView(this);
    int index = addTab(view, "");
    setCurrentIndex(index);
    view->setUrl(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
}

void TabWidget::tabChanged(int index)
{
    QWidget * widget = this->widget(index);
    Q_ASSERT(widget);
    EditorView * view = qobject_cast<EditorView *>(widget);
    Q_ASSERT(view);
//    qDebug() << index << currentFiles.value(widget);
    m_previousWidget = m_currentWidget;
    m_currentWidget = widget;
    emit currentChanged(m_currentWidget);

//    if (m_previousWidget)
//        disconnectView(previousWidget);
//    connectView(view);
//    connectEditor(view->centralWidget());
//    setAddress(view->currentUrl());
}

void TabWidget::closeTab(int index)
{
#warning hardcoded
    if (this->count() == 1) // do not allow to close last tab
        return;

    QWidget * widget = this->widget(index);
    Q_ASSERT(widget);
    EditorView * view = qobject_cast<EditorView *>(widget);
    Q_ASSERT(view);
    this->removeTab(index);
    delete view;
}
