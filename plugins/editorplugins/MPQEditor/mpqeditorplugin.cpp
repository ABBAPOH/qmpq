#include "mpqeditorplugin.h"

#include <QtCore/QtPlugin>
#include <QtGui/QToolBar>
#include <QtGui/QFileDialog>
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QMenu>
#include <QtGui/QContextMenuEvent>
//#include <QDebug>

#include "mpqeditor.h"

//================================== MPQEditorPlugin ==================================

MPQEditorPlugin::MPQEditorPlugin(MPQEditor * editor)
    : viewModeMapper(new QSignalMapper()),
    viewModeActionGroup(new QActionGroup(this))
{
    m_editor = editor;
    m_toolBar = new QToolBar();

    m_editor->installEventFilter(this);

//    m_toolBar->addAction(QIcon(":/icons/images/prev.png"), "back", this, SLOT(back()));
//    m_toolBar->addAction(QIcon(":/icons/images/next.png"), "forward", this, SLOT(forward()));
//    m_toolBar->addAction(QIcon(":/icons/images/up.png"), "up", m_editor, SLOT(up()));
//    m_toolBar->addSeparator();
    actionAdd = m_toolBar->addAction(QIcon(":/icons/images/add.png"), "Add", this, SLOT(add()));
    actionExtract = m_toolBar->addAction(QIcon(":/icons/images/extract.png"), "Extract", this, SLOT(extract()));
    actionRename = new QAction(QIcon(":/icons/images/rename.png"), "Rename", m_toolBar);
    connect(actionRename, SIGNAL(triggered()), m_editor, SLOT(rename()));
    actionRemove = m_toolBar->addAction(QIcon(":/icons/images/remove.png"), "Remove", m_editor, SLOT(remove()));
    m_toolBar->addSeparator();
    viewModeActions[0] = new QAction(QIcon(":/icons/images/list.png"), "listView", m_toolBar);
    viewModeActions[1] = new QAction(QIcon(":/icons/images/icons.png"), "iconView", m_toolBar);
    viewModeActions[2] = new QAction(QIcon(":/icons/images/table.png"), "tableView", m_toolBar);
    viewModeActions[3] = new QAction(QIcon(":/icons/images/column.png"), "columnView", m_toolBar);
    viewModeActions[4] = new QAction(QIcon(":/icons/images/treeview.png"), "treeView", m_toolBar);

    for (int i = 0; i < MaxViews; i++) {
        m_toolBar->addAction(viewModeActions[i]);
        connect(viewModeActions[i], SIGNAL(triggered()), viewModeMapper, SLOT(map()));
        viewModeMapper->setMapping(viewModeActions[i], i);
        viewModeActions[i]->setCheckable(true);
        viewModeActionGroup->addAction(viewModeActions[i]);
    }
    #warning TODO: why uses SLOT in this class instead of slot in MPQEditor
    connect(viewModeMapper, SIGNAL(mapped(int)), this, SLOT(setViewMode(int)));
    viewModeActions[0]->trigger();

    actionNew_Folder = new QAction("new Folder", this);
    connect(actionNew_Folder, SIGNAL(triggered()), m_editor, SLOT(newFolder()));

//    m_toolBar->addAction("listView", this, SLOT(setListViewMode()));
//    m_toolBar->addAction("iconView", this, SLOT(setIconViewMode()));
//    m_toolBar->addAction("columnView", this, SLOT(setColumnViewMode()));
//    m_toolBar->addAction("tableView", this, SLOT(setTableViewMode()));
//    m_toolBar->addAction("treeView", this, SLOT(setTreeViewMode()));
}

MPQEditorPlugin::~MPQEditorPlugin()
{
//    qDebug("MPQEditorPlugin::~MPQEditorPlugin()");
//    qDebug() << (long long)this;
    delete m_editor;
}

bool MPQEditorPlugin::open(const QString &file)
{
    m_editor->open(file);
    return true;
}

void MPQEditorPlugin::close()
{
    m_editor->closeFile();
}

void MPQEditorPlugin::add()
{
    QStringList files = QFileDialog::getOpenFileNames(m_editor, tr("Select Files to add"));
    m_editor->add(files);
}

void MPQEditorPlugin::extract()
{
    QString dir = QFileDialog::getExistingDirectory(m_editor, tr("Select Target Directory"));
    m_editor->extract(dir);
}

void MPQEditorPlugin::setViewMode(int mode)
{
//    qDebug("MPQEditorPlugin::setViewMode");
    m_editor->setViewMode((MPQEditor::ViewMode)mode);

}

QWidget * MPQEditorPlugin::widget()
{
    return m_editor;
}

QToolBar * MPQEditorPlugin::toolBar ()
{
    return m_toolBar;
}

bool MPQEditorPlugin::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::ContextMenu) {
//        qDebug("context menu called");
        return contextMenu(static_cast<QContextMenuEvent*>(event));
    }
    return QObject::eventFilter(obj, event);
}

bool MPQEditorPlugin::contextMenu(QContextMenuEvent *event)
{
    QMenu menu;
//    QList<QAction*> list;
    menu.addAction(actionNew_Folder);
    menu.addSeparator();
    menu.addAction(actionAdd);
    menu.addAction(actionExtract);
    menu.addAction(actionRename);
    menu.addAction(actionRemove);
    menu.exec(event->globalPos());
    return true;
}

//================================== MPQEditorFactory ==================================

MPQEditorFactory::MPQEditorFactory()
{

}

//IEditor * MPQEditorFactory::instance(QWidget * parent)
IEditor * MPQEditorFactory::createEditor(QWidget * parent)
{
//    qDebug("MPQEditorFactory::instance");
    MPQEditor * editor = new MPQEditor(parent);
    return new MPQEditorPlugin(editor);
}

void MPQEditorFactory::shutdown()
{
    qDebug("MPQEditorFactory::shutdown");
    delete MPQEditor::model();
//    MPQEditor::model()->deleteLater();;
}

bool MPQEditorFactory::canHandle(const QString &file) const
{
//    qDebug("MPQEditorFactory::canHandle");
    if (file == "" || QFileInfo(file).isDir())
        return true;
    else
        return false;
}

Q_EXPORT_PLUGIN2(mpq_editor_factory, MPQEditorFactory)
