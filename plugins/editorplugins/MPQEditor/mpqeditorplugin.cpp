#include "mpqeditorplugin.h"

#include <QtCore/QtPlugin>
#include <QtGui/QToolBar>
#include <QtGui/QFileDialog>
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
#include <QDebug>

#include "mpqeditor.h"

//================================== MPQEditorPlugin ==================================

MPQEditorPlugin::MPQEditorPlugin(MPQEditor * editor)
    : viewModeMapper(new QSignalMapper())
{
    m_editor = editor;
    m_toolBar = new QToolBar();

//    m_toolBar->addAction(QIcon(":/icons/images/prev.png"), "back", this, SLOT(back()));
//    m_toolBar->addAction(QIcon(":/icons/images/next.png"), "forward", this, SLOT(forward()));
//    m_toolBar->addAction(QIcon(":/icons/images/up.png"), "up", m_editor, SLOT(up()));
//    m_toolBar->addSeparator();
    m_toolBar->addAction(QIcon(":/icons/images/add.png"), "add", this, SLOT(add()));
    m_toolBar->addAction(QIcon(":/icons/images/extract.png"), "extract", this, SLOT(extract()));
    m_toolBar->addAction(QIcon(":/icons/images/remove.png"), "remove", m_editor, SLOT(remove()));
    m_toolBar->addSeparator();
    viewModeActions[0] = new QAction("listView", m_toolBar);
    viewModeActions[1] = new QAction("iconView", m_toolBar);
    viewModeActions[2] = new QAction("tableView", m_toolBar);
    viewModeActions[3] = new QAction("columnView", m_toolBar);
    viewModeActions[4] = new QAction("treeView", m_toolBar);
    for (int i = 0; i < 5; i++) {
        m_toolBar->addAction(viewModeActions[i]);
        connect(viewModeActions[i], SIGNAL(triggered()), viewModeMapper, SLOT(map()));
        viewModeMapper->setMapping(viewModeActions[i], i);
    }
    connect(viewModeMapper, SIGNAL(mapped(int)), this, SLOT(setViewMode(int)));


//    m_toolBar->addAction("listView", this, SLOT(setListViewMode()));
//    m_toolBar->addAction("iconView", this, SLOT(setIconViewMode()));
//    m_toolBar->addAction("columnView", this, SLOT(setColumnViewMode()));
//    m_toolBar->addAction("tableView", this, SLOT(setTableViewMode()));
//    m_toolBar->addAction("treeView", this, SLOT(setTreeViewMode()));
}

MPQEditorPlugin::~MPQEditorPlugin()
{
    qDebug("MPQEditorPlugin::~MPQEditorPlugin()");
    qDebug() << (long long)this;
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

//================================== MPQEditorFactory ==================================

MPQEditorFactory::MPQEditorFactory()
{

}

QStringList MPQEditorFactory::formats()
{
    return QStringList() << "*.mpq" << "*.w3x";
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
}

bool MPQEditorFactory::canHandle(const QString &file) const
{
    qDebug("MPQEditorFactory::canHandle");
    if (file == "" || QFileInfo(file).isDir())
        return true;
    else
        return false;
}

Q_EXPORT_PLUGIN2(mpq_editor_factory, MPQEditorFactory)

