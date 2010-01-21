#include "editorview.h"

#include "editormanager.h"
#include "ieditor.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBar>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QSet>
#include <QApplication>

#include <QDebug>

EditorView::EditorView(QWidget *parent) :
    QStackedWidget(parent), editorManager(new EditorManager(this)), m_centralWidget(0), historyPos(-1)
{
//    openUrl("");
}

EditorView::~EditorView()
{
//    foreach (IEditor * editor, m_editors.values().toSet()) {
////        qDebug() << "deleting " << (long long)editor;
//        editor->close();
//        delete editor;
//    }
//    m_editors.clear();
#warning check deletion
    delete editorManager;
}

QWidget * EditorView::createWidgetForEditor(IEditor * editor)
{
    qDebug() << "EditorView::createWidgetForEditor" << (long long)editor;
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
    qDebug("EditorView::back");
    qDebug() << m_history << historyPos;
    if (historyPos == 0)
        return;
    historyPos--;
    setUrl(m_history.at(historyPos));
}

void EditorView::forward()
{
    qDebug("EditorView::forward");
    qDebug() << m_history << historyPos;
    if (historyPos == m_history.count() - 1)
        return;
    historyPos++;
    setUrl(m_history.at(historyPos));
}

void EditorView::up()
{
    QFileInfo info(m_current);
    if (info.isRoot())
        setUrl("");
    else
        setUrl(info.path());
}

//cleans all history from start to end of list
void EditorView::cleanHistory(int start)
{
    qDebug("EditorView::cleanHistory");
    for (int i = start, end = m_history.count(); i < end; i++) {
        QString path = m_history.at(start);

        // if trying to go manually to some place in removing part, do not try to release editor
        if (path != m_current) {
            IEditor * editor = editorManager->editor(path);
            editorManager->close(path);
            if (!editorManager->isOpened(editor)) {
                delete m_widgets.value(editor);
                m_widgets.remove(editor);
            }
        }
        m_history.removeAt(start);
    }
    historyPos = start - 1;
    qDebug() << m_history;
}

bool EditorView::openUrl(const QString & url)
{
//    if (m_current.right(1) == "/")
//        m_current = m_current.left(m_current.length() - 1);
    IEditor * editor = editorManager->open(url);
    if (!editor)
        return false;
    m_current = url;

    if (historyPos == -1) {
        m_history.append(url);
        historyPos++;
    } else if (url != m_history.at(historyPos)) {
        cleanHistory(historyPos+1);
        m_history.append(url);
        historyPos++;
    }

//    if (!m_editors.contains(url))
//        m_editors.insert(url, editor);
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
        if (m_centralWidget)
            m_centralWidget->clearFocus();
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
