#include "mpqeditorplugin.h"

#include <QtCore/QtPlugin>

#include <QtGui/QApplication>
#include <QtGui/QToolBar>
#include <QtGui/QFileDialog>
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QMenu>
#include <QtGui/QContextMenuEvent>
//#include <QDebug>

#include "mpqeditor.h"
#include "idirmodel.h"

//================================== MPQEditorPlugin ==================================

MPQEditorInterface::MPQEditorInterface(MPQEditor * editor)
    : viewModeMapper(new QSignalMapper()),
    viewModeActionGroup(new QActionGroup(this))
{
    m_editor = editor;
    m_toolBar = new QToolBar();

    m_editor->installEventFilter(this);

    initActions();

    connect(m_editor, SIGNAL(openRequested(QString)), SLOT(openRequest(QString)));
    connect(m_editor, SIGNAL(currentChanged(QString)), SLOT(openRequest(QString)));
}

MPQEditorInterface::~MPQEditorInterface()
{
    delete m_editor;
}

void MPQEditorInterface::initActions()
{
    actionAdd = m_toolBar->addAction(QIcon(":/icons/images/add.png"), tr("Add"), this, SLOT(add()));
    actionExtract = m_toolBar->addAction(QIcon(":/icons/images/extract.png"), tr("Extract"), this, SLOT(extract()));
    actionRename = new QAction(QIcon(":/icons/images/rename.png"), tr("Rename"), m_toolBar);
    connect(actionRename, SIGNAL(triggered()), m_editor, SLOT(rename()));
    actionRemove = m_toolBar->addAction(QIcon(":/icons/images/remove.png"), tr("Remove"), m_editor, SLOT(remove()));
    m_toolBar->addSeparator();
    viewModeActions[0] = new QAction(QIcon(":/icons/images/list.png"), tr("List View"), m_toolBar);
    viewModeActions[1] = new QAction(QIcon(":/icons/images/icons.png"), tr("Icon View"), m_toolBar);
    viewModeActions[2] = new QAction(QIcon(":/icons/images/table.png"), tr("Table View"), m_toolBar);
    viewModeActions[3] = new QAction(QIcon(":/icons/images/column.png"), tr("Column View"), m_toolBar);
    viewModeActions[4] = new QAction(QIcon(":/icons/images/treeview.png"), tr("Tree View"), m_toolBar);

    for (int i = 0; i < MaxViews; i++) {
        m_toolBar->addAction(viewModeActions[i]);
        connect(viewModeActions[i], SIGNAL(triggered()), viewModeMapper, SLOT(map()));
        viewModeMapper->setMapping(viewModeActions[i], i);
        viewModeActions[i]->setCheckable(true);
        viewModeActionGroup->addAction(viewModeActions[i]);
    }

    // i use slot in this class because moc can't convert int to enum
    connect(viewModeMapper, SIGNAL(mapped(int)), SLOT(setViewMode(int)));
    viewModeActions[0]->trigger();

    actionOpen = new QAction(tr("Open"), this);
    actionOpen->setShortcut(tr("Return"));
    connect(actionOpen, SIGNAL(triggered()), SLOT(open()));
    m_editor->addAction(actionOpen);

    actionOpenInNewTab = new QAction(tr("Open in New Tab"), this);
    actionOpenInNewTab->setShortcut(tr("Ctrl+Return"));
    connect(actionOpenInNewTab, SIGNAL(triggered()), SLOT(openInNewTab()));
    m_editor->addAction(actionOpenInNewTab);

    actionOpenInNewWindow = new QAction(tr("Open in New Window"), this);
    actionOpenInNewWindow->setShortcut(tr("Shift+Return"));
    connect(actionOpenInNewWindow, SIGNAL(triggered()), SLOT(openInNewWindow()));
    m_editor->addAction(actionOpenInNewWindow);

    actionOpenExternally = new QAction(tr("Open in External Program"), this);
    actionOpenExternally->setShortcut(tr("Meta+Return"));
    connect(actionOpenExternally, SIGNAL(triggered()), SLOT(openExternally()));
    m_editor->addAction(actionOpenExternally);

    actionReopen = new QAction(tr("Reopen using listfile..."), this);
    connect(actionReopen, SIGNAL(triggered()), SLOT(reopen()));

    actionApply = new QAction(tr("Apply changes to archive"), this);
    connect(actionApply, SIGNAL(triggered()), SLOT(submit()));

    actionNew_Folder = new QAction(tr("New Folder"), this);
    connect(actionNew_Folder, SIGNAL(triggered()), m_editor, SLOT(newFolder()));

}

bool MPQEditorInterface::canHandle(const QString & filePath)
{
    return MPQEditorPlugin::canHandle(filePath);
}

QString MPQEditorInterface::currentFile()
{
    return m_editor->currentFile();
}

bool MPQEditorInterface::open(const QString &file)
{
    m_editor->open(file);
    return true;
}

void MPQEditorInterface::close()
{
    m_editor->closeFile();
}

void MPQEditorInterface::add()
{
    QStringList files = QFileDialog::getOpenFileNames(m_editor, tr("Select Files to add"));
    m_editor->add(files);
}

void MPQEditorInterface::extract()
{
    QString dir = QFileDialog::getExistingDirectory(m_editor, tr("Select Target Directory"));
    m_editor->extract(dir);
}

void MPQEditorInterface::setViewMode(int mode)
{
    m_editor->setViewMode((UniversalView::ViewMode)mode);
}

void MPQEditorInterface::openRequest(const QString & path)
{
    ICore::instance()->windowManager()->open(path);
}

void MPQEditorInterface::open()
{
    QStringList paths = m_editor->selectedPaths();
    if (paths.isEmpty())
        return;
    ICore::instance()->windowManager()->open(paths.first());
    for (int i = 1; i < paths.count(); i++) {
        ICore::instance()->windowManager()->openInNewTab(paths.at(i));
    }
}

void MPQEditorInterface::openInNewTab()
{
    QStringList paths = m_editor->selectedPaths();
    for (int i = 0; i < paths.count(); i++) {
        ICore::instance()->windowManager()->openInNewTab(paths.at(i));
    }
}

void MPQEditorInterface::openInNewWindow()
{
    QStringList paths = m_editor->selectedPaths();
    for (int i = 0; i < paths.count(); i++) {
        ICore::instance()->windowManager()->openInNewWindow(paths.at(i));
    }
}

void MPQEditorInterface::openExternally()
{
    QStringList paths = m_editor->selectedPaths();
    for (int i = 0; i < paths.count(); i++) {
        ICore::instance()->windowManager()->openExternally(paths.at(i));
    }
}

void MPQEditorInterface::reopen()
{
    QString filePath = QFileDialog::getOpenFileName(m_editor, tr("Select listfile"));
    if (filePath.isEmpty())
        return;
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    m_editor->reopenUsingListfile(file.readAll());
    file.close();
}

QWidget * MPQEditorInterface::widget()
{
    return m_editor;
}

QToolBar * MPQEditorInterface::toolBar ()
{
    return m_toolBar;
}

bool MPQEditorInterface::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::ContextMenu) {
        return contextMenu(static_cast<QContextMenuEvent*>(event));
    }
    return QObject::eventFilter(obj, event);
}

bool MPQEditorInterface::contextMenu(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(actionOpen);
    menu.addAction(actionOpenInNewTab);
    menu.addAction(actionOpenInNewWindow);
    menu.addAction(actionOpenExternally);
    menu.addSeparator();

    menu.addAction(actionNew_Folder);
    menu.addSeparator();

    QMenu mpqMenu;
    mpqMenu.setTitle("MPQ");
    mpqMenu.addAction(actionReopen);
    mpqMenu.addAction(actionApply);

    QStringList paths = m_editor->selectedPaths();
    if (!paths.isEmpty() && m_editor->isMPQArchive(paths.first())) {
        actionReopen->setEnabled(true);
    } else {
        actionReopen->setEnabled(false);
    }
    actionApply->setEnabled(false);

    menu.addMenu(&mpqMenu);
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
    return new MPQEditorInterface(editor);
}

void MPQEditorFactory::shutdown()
{
//    qDebug("MPQEditorFactory::shutdown");
//    delete MPQEditor::model();
//    MPQEditor::model()->deleteLater();;
}

bool MPQEditorFactory::canHandle(const QString &file) const
{
    return MPQEditorPlugin::canHandle(file);
}

//================================== MPQEditorPlugin ==================================

void MPQEditorPlugin::initialize()
{
    ICore::instance()->editorFactoryManager()->addFactory(new MPQEditorFactory);
    ICore::instance()->fileManager()->registerExtensionString(
            "MPQ Archives (*.mpq *.w3x *.w3m *.s2ma *.SC2Data *.SC2Archive *.SC2Assets *.SC2Replay *.scx *.w3n *.snp *.sv *.hsv)"
            );
}

void MPQEditorPlugin::shutdown()
{
    qDebug("MPQEditorPlugin::shutdown");
//    delete MPQEditor::model();
    delete FileSystemModelWrapper::modelInstance();
}

bool MPQEditorPlugin::canHandle(const QString &file)
{
//    qDebug("MPQEditorFactory::canHandle");
    if (file == "" || QFileInfo(file).isDir())
        return true;
    else
        return false;
}

//Q_EXPORT_PLUGIN2(mpq_editor_factory, MPQEditorFactory)
Q_EXPORT_PLUGIN(MPQEditorPlugin)
