#include "editorview.h"

#include "editormanager.h"
#include "ieditor.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBar>
#include <QtCore/QFileInfo>
#include <QtCore/QSet>

//#include <QDebug>

EditorView::EditorView(QWidget *parent) :
    QStackedWidget(parent), editorManager(new EditorManager(this))
{
    openUrl("");
}

EditorView::~EditorView()
{
    foreach (IEditor * editor, m_editors.values().toSet()) {
//        qDebug() << "deleting " << (long long)editor;
        editor->close();
        delete editor;
    }
    m_editors.clear();
}

QWidget * EditorView::createWidgetForEditor(IEditor * editor)
{
    QWidget * widget = new QWidget();
    QVBoxLayout * lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(editor->toolBar());
    editor->toolBar()->setMaximumHeight(24);
    lay->addWidget(editor->widget());
    widget->setLayout(lay);
    return widget;
}

void EditorView::back()
{
}

void EditorView::forward()
{
}

void EditorView::up()
{
    QFileInfo info(m_current);
    if (info.isRoot())
        setUrl("");
    else
        setUrl(info.path());
}

bool EditorView::openUrl(const QString & url)
{
//    if (m_current.right(1) == "/")
//        m_current = m_current.left(m_current.length() - 1);
    IEditor * editor = editorManager->open(url);
    if (!editor)
        return false;
    m_current = url;
    if (!m_editors.contains(url))
        m_editors.insert(url, editor);
    QWidget * widget;
    if (m_widgets.contains(editor)) {
        widget = m_widgets.value(editor);
    } else {
        widget = createWidgetForEditor(editor);
        m_widgets.insert(editor, widget);
        addWidget(widget);
        connect(editor->widget(), SIGNAL(currentChanged(const QString &)), SLOT(setUrl(const QString &)));
    }
    setCurrentWidget(widget);
    setCentralWidget(editor->widget());
    return true;
}

void EditorView::setCentralWidget(QWidget * widget)
{
    if (m_centralWidget != widget) {
        m_centralWidget = widget;
        emit centralWidgetChanged(widget);
    }
}

void EditorView::setUrl(const QString & url)
{
    if (QFileInfo(m_current) == QFileInfo(url)) // ignores / in the end of the path
        return;

    if (openUrl(url))
        emit currentUrlChanged(url);
}
