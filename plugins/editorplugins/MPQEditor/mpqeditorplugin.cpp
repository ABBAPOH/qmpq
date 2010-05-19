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

#include "../../../../QMPQFileEngine/qmpqfileengine.h"
#include "../../../../QMPQFileEngine/qmpqarchive.h"

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
    IActionManager * manager = ICore::instance()->actionManager();

    m_toolBar->addAction(manager->action(Core::ACTION_ADD));
    m_toolBar->addAction(manager->action(Core::ACTION_EXTRACT));
    m_toolBar->addAction(manager->action(Core::ACTION_RENAME));
    m_toolBar->addAction(manager->action(Core::ACTION_REMOVE));
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

    actionNew_Folder = new QAction(tr("New Folder"), this);
    connect(actionNew_Folder, SIGNAL(triggered()), m_editor, SLOT(newFolder()));

}

bool MPQEditorInterface::canHandle(const QString & filePath)
{
    return MPQEditorPlugin::canHandle(filePath);
}

void MPQEditorInterface::close()
{
    m_editor->closeFile();
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
    IActionManager * manager = ICore::instance()->actionManager();

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
    mpqMenu.addAction(manager->action(Core::ACTION_REOPEN));
    mpqMenu.addAction(manager->action(Core::ACTION_COMPACT));
    mpqMenu.addAction(manager->action(Core::ACTION_SET_HASH_TABLE_SIZE));

    menu.addMenu(&mpqMenu);
    menu.addSeparator();
    menu.addAction(manager->action(Core::ACTION_ADD));
    menu.addAction(manager->action(Core::ACTION_EXTRACT));
    menu.addAction(manager->action(Core::ACTION_RENAME));
    menu.addAction(manager->action(Core::ACTION_REMOVE));
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

    initActions();

    connect(ICore::instance()->editorFactoryManager(), SIGNAL(currentEditorChanged(IEditor*)), SLOT(updateActions()));
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

MPQEditorInterface * MPQEditorPlugin::editor()
{
    ICore * core = ICore::instance();
    return qobject_cast<MPQEditorInterface *>(core->editorFactoryManager()->currentEditor());
}

MPQEditor * MPQEditorPlugin::editorWidget()
{
    MPQEditorInterface * editor = this->editor();
    if (editor)
        return static_cast<MPQEditor*>(editor->widget());
}

void MPQEditorPlugin::updateActions()
{
    qDebug("MPQEditorPlugin::updateActions");
    ICore * core = ICore::instance();
    MPQEditorInterface * editor = this->editor();
    core->actionManager()->action(Core::ACTION_ADD)->setEnabled(editor);
    core->actionManager()->action(Core::ACTION_EXTRACT)->setEnabled(editor);
    core->actionManager()->action(Core::ACTION_RENAME)->setEnabled(editor);
    core->actionManager()->action(Core::ACTION_REMOVE)->setEnabled(editor);

    bool b = editor && editor->currentFile().startsWith("mpq:");
    core->actionManager()->action(Core::ACTION_REOPEN)->setEnabled(b);
    core->actionManager()->action(Core::ACTION_COMPACT)->setEnabled(b);
    core->actionManager()->action(Core::ACTION_SET_HASH_TABLE_SIZE)->setEnabled(b);
}

void MPQEditorPlugin::add()
{
    MPQEditor * editor = editorWidget();
    QStringList files = QFileDialog::getOpenFileNames(editor, tr("Select Files to add"));
    editor->add(files);
}

void MPQEditorPlugin::extract()
{
    MPQEditor * editor = editorWidget();
    QString dir = QFileDialog::getExistingDirectory(editor, tr("Select Target Directory"));
    editor->extract(dir);
}

void MPQEditorPlugin::rename()
{
    editorWidget()->rename();
}

void MPQEditorPlugin::remove()
{
    editorWidget()->rename();
}

void MPQEditorPlugin::reopen()
{
    MPQEditor * editor = editorWidget();
    QString filePath = QFileDialog::getOpenFileName(editor, tr("Select listfile"));
    if (filePath.isEmpty())
        return;
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    editor->reopenUsingListfile(file.readAll());
    file.close();
}

#include "compactprocessdialog.h"
void MPQEditorPlugin::compact()
{
    MPQEditor * editor = editorWidget();
    QString filePath = editor->currentFile();
    QFile file(filePath);
    //  if we can cat file then we're in mpq archive
    QMPQFileEngine * engine = dynamic_cast<QMPQFileEngine *>(file.fileEngine());
    if (engine) {
        QMPQArchive * archive = engine->archive();
        if (archive) {
            QString archivePath = archive->file();
            CompactProcessDialog dlg;
            connect(archive, SIGNAL(compactProcessChanged(QMPQArchive::CompactOperation,qint64,qint64)),
                    &dlg, SLOT(setProgress(QMPQArchive::CompactOperation, qint64, qint64)));
            dlg.show();
            archive->compact();
//            archive->closeArchive();
//            archive->openArchive(archivePath);
        }
    }
}

void MPQEditorPlugin::initActions()
{
    IActionManager * manager = ICore::instance()->actionManager();

    actionAdd = manager->action(Core::ACTION_ADD);
    actionAdd->setIcon(QIcon(":/icons/images/add.png"));
    actionAdd->setText(tr("Add"));
    connect(actionAdd, SIGNAL(triggered()), SLOT(add()));

    actionExtract = manager->action(Core::ACTION_EXTRACT);
    actionExtract->setIcon(QIcon(":/icons/images/extract.png"));
    actionExtract->setText(tr("Extract"));
    connect(actionExtract, SIGNAL(triggered()), SLOT(extract()));

    actionRename = manager->action(Core::ACTION_RENAME);
    actionRename->setIcon(QIcon(":/icons/images/rename.png"));
    actionRename->setText(tr("Rename"));
    connect(actionRename, SIGNAL(triggered()), SLOT(rename()));

    actionRemove = manager->action(Core::ACTION_REMOVE);
    actionRemove->setIcon(QIcon(":/icons/images/remove.png"));
    actionRemove->setText(tr("Remove"));
    connect(actionRemove, SIGNAL(triggered()), SLOT(remove()));

    actionReopen = manager->action(Core::ACTION_REOPEN);
    actionReopen->setText(tr("Reopen using listfile..."));
    connect(actionReopen, SIGNAL(triggered()), SLOT(reopen()));

    actionCompact = manager->action(Core::ACTION_COMPACT);
    actionCompact->setText(tr("Compact archive"));
    actionCompact->setEnabled(false);
    connect(actionCompact, SIGNAL(triggered()), SLOT(compact()));

    actionSetHashTableSize = manager->action(Core::ACTION_SET_HASH_TABLE_SIZE);
    actionSetHashTableSize->setText(tr("Set hash table size..."));
    actionSetHashTableSize->setEnabled(false);
    connect(actionSetHashTableSize, SIGNAL(triggered()), SLOT(setHashTableSize()));

    QMenu * toolsMenu = manager->menu(Core::MENU_TOOLS);
    QMenu * menu = new QMenu("MPQ Viewer");

    QMenu * mpqMenu = new QMenu;
    mpqMenu->setTitle("MPQ");
    mpqMenu->addAction(actionReopen);
    mpqMenu->addAction(actionCompact);
    mpqMenu->addAction(actionSetHashTableSize);

    menu->addMenu(mpqMenu);
    menu->addSeparator();
    menu->addAction(actionAdd);
    menu->addAction(actionExtract);
    menu->addAction(actionRename);
    menu->addAction(actionRemove);
    toolsMenu->addMenu(menu);
}

//Q_EXPORT_PLUGIN2(mpq_editor_factory, MPQEditorFactory)
Q_EXPORT_PLUGIN(MPQEditorPlugin)
