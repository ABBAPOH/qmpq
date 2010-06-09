#include "mpqeditorplugin.h"

#include <QtCore/QtPlugin>
#include <QtCore/QMetaEnum>

#include <QtGui/QApplication>
#include <QtGui/QToolBar>
#include <QtGui/QFileDialog>
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QMenu>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QLineEdit>
#include <QMessageBox>
//#include <QDebug>

#include "mpqeditor.h"
#include "idirmodel.h"
#include "compactprocessdialog.h"
#include "changeattributesdialog.h"
#include "hashtablesizedialog.h"
#include "verifyfilesdialog.h"
#include "archivepropertiesdialog.h"
#include "filepropertiesdialog.h"

#include "../../../../QMPQFileEngine/qmpqfileengine.h"
#include "../../../../QMPQFileEngine/qmpqarchiveex.h"
#include "../../../../QMPQFileEngine/mpqfileinfo.h"

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

    QLineEdit * filterLineEdit = new QLineEdit(m_toolBar);
    filterLineEdit->setText("*");
    connect(filterLineEdit, SIGNAL(textChanged(QString)), m_editor, SLOT(setNameFilter(const QString &)));
    m_toolBar->addSeparator();
    m_toolBar->addWidget(filterLineEdit);

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
    actionOpenExternally->setShortcut(tr("Alt+Return"));
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
    bool result = ICore::instance()->windowManager()->open(path);
    if (!result) {
        result = ICore::instance()->windowManager()->openExternally(path);
        if (!result) {
            QMessageBox::warning(0, "Warning", tr("Can't open file"));
        }
    }
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
    return 0;
}


QMPQArchiveEx * MPQEditorPlugin::getArchive(const QString & filePath)
{
    QFile file(filePath);
    //  if we can cat file then we're in mpq archive
    QMPQFileEngine * engine = dynamic_cast<QMPQFileEngine *>(file.fileEngine());
    if (engine)
        return engine->archive();
    else
        return 0;
}

void MPQEditorPlugin::updateActions()
{
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
    core->actionManager()->action(Core::ACTION_CHANGE_ATTRIBUTES)->setEnabled(b);
    core->actionManager()->action(Core::ACTION_VERIFY_ARCHIVE)->setEnabled(b);
    core->actionManager()->action(Core::ACTION_VERIFY_FILES)->setEnabled(b);
    core->actionManager()->action(Core::ACTION_ARCHIVE_PROPERTIES)->setEnabled(b);
    core->actionManager()->action(Core::ACTION_FILE_PROPERTIES)->setEnabled(b);
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
    editorWidget()->remove();
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

void MPQEditorPlugin::compact()
{
    MPQEditor * editor = editorWidget();
    QMPQArchiveEx * archive = getArchive(editor->currentFile());
    if (archive) {
        CompactProcessDialog dlg;
        connect(archive, SIGNAL(compactProcessChanged(QMPQArchive::CompactOperation,qint64,qint64)),
                &dlg, SLOT(setProgress(QMPQArchive::CompactOperation, qint64, qint64)));
        dlg.show();
        archive->compact();
        //            archive->closeArchive();
        //            archive->openArchive(archivePath);
    }
}

void MPQEditorPlugin::setHashTableSize()
{
    MPQEditor * editor = editorWidget();
    QMPQArchiveEx * archive = getArchive(editor->currentFile());
    if (archive) {
        HashTableSizeDialog dlg;
        dlg.setHashTableSize(archive->hashTableSize());
        int result = dlg.exec();
        if (result == QDialog::Accepted) {
            archive->setHashTableSize(dlg.hashTableSize());
        }
    }
}

void MPQEditorPlugin::changeAttributes()
{
    MPQEditor * editor = editorWidget();
    QMPQArchiveEx * archive = getArchive(editor->currentFile());
    if (!archive) {
        return;
    }

    ChangeAttributesDialog dlg;

    dlg.setAttributes(archive->attributes());

    int result = dlg.exec();
    if (result == QDialog::Accepted) {
        archive->setAttributes(dlg.attributes());
        if (dlg.updateFiles()) {
            foreach (MPQFileInfo info, archive->entryList()) {
                archive->updateFileAttributes(info.name());
            }
        }
    }
}

void MPQEditorPlugin::verifyArchive()
{
    MPQEditor * editor = editorWidget();
    QMPQArchiveEx * archive = getArchive(editor->currentFile());
    if (archive) {
        QMPQArchive::VerifyArchiveError result = archive->verifyArchive();
        QString message;
        switch(result) {
        case QMPQArchive::NoSignature :
            message = tr("Archive has no digital signature."); break;
        case QMPQArchive::VerifyFailed :
            message = tr("Archive verification failed."); break;
        case QMPQArchive::WeakSignatureOk :
            message = tr("Weak signature is ok."); break;
        case QMPQArchive::WeakSignatureError :
            message = tr("Weak signature error."); break;
        case QMPQArchive::StrongSignatureOk :
            message = tr("Strong signature is ok."); break;
        case QMPQArchive::StrongSignatureError :
            message = tr("Strong signature error."); break;
        }

        QMessageBox::information(editor, tr("Verify Archive"), message);
    }
}

void MPQEditorPlugin::verifyFiles()
{
    MPQEditor * editor = editorWidget();
    QMPQArchiveEx * archive = getArchive(editor->currentFile());
    if (!archive)
        return;

    bool ok = true;
    VerifyFilesDialog * dlg = new VerifyFilesDialog;
    dlg->setAttribute(Qt::WA_DeleteOnClose, true);
    dlg->show();
    foreach (MPQFileInfo info, archive->entryList()) {
        QMPQArchive::VerifyFileFlags result = archive->verifyFile(info.name(), QFlag(QMPQArchive::CRC32 | QMPQArchive::MD5));
        if (result != QMPQArchive::VerifyFileOk) {
            ok = false;
            QMetaObject mo = QMPQArchive::staticMetaObject;
            QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("VerifyFileFlags"));
            QString message = me.valueToKeys(result);
            dlg->addLine(info.name(), message);
        }
    }
    if (ok)
        dlg->addLine(tr("All files are ok"), "");
}

void MPQEditorPlugin::archiveProperties()
{
    MPQEditor * editor = editorWidget();
    QMPQArchiveEx * archive = getArchive(editor->currentFile());
    if (!archive)
        return;

    ArchivePropertiesDialog dlg;
    dlg.setArchive(archive);
    dlg.exec();
}

void MPQEditorPlugin::fileProperties()
{
    MPQEditor * editor = editorWidget();
    QMPQArchiveEx * archive = getArchive(editor->currentFile());
    if (!archive)
        return;
    QStringList paths = editor->selectedPaths();
    foreach (QString path, paths) {
        FilePropertiesDialog dlg;
        #warning "TODO: find other way of determining inner file path"
        path = path.mid(archive->file().length()+5);
        qDebug() << path;
        dlg.setInfo(archive, path);
        dlg.exec();
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

    actionChangeAttributes = manager->action(Core::ACTION_CHANGE_ATTRIBUTES);
    actionChangeAttributes->setText(tr("Change Attributes..."));
    actionChangeAttributes->setEnabled(false);
    connect(actionChangeAttributes, SIGNAL(triggered()), SLOT(changeAttributes()));

    actionVerifyArchive = manager->action(Core::ACTION_VERIFY_ARCHIVE);
    actionVerifyArchive->setText(tr("Verify archive..."));
    actionVerifyArchive->setEnabled(false);
    connect(actionVerifyArchive, SIGNAL(triggered()), SLOT(verifyArchive()));

    actionVerifyFiles = manager->action(Core::ACTION_VERIFY_FILES);
    actionVerifyFiles->setText(tr("Verify file(s) in archive..."));
    actionVerifyFiles->setEnabled(false);
    connect(actionVerifyFiles, SIGNAL(triggered()), SLOT(verifyFiles()));

    actionArchiveProperties = manager->action(Core::ACTION_ARCHIVE_PROPERTIES);
    actionArchiveProperties->setText(tr("Archive properties..."));
    actionArchiveProperties->setEnabled(false);
    connect(actionArchiveProperties, SIGNAL(triggered()), SLOT(archiveProperties()));

    actionFileProperties = manager->action(Core::ACTION_FILE_PROPERTIES);
    actionFileProperties->setText(tr("File properties..."));
    actionFileProperties->setEnabled(false);
    connect(actionFileProperties, SIGNAL(triggered()), SLOT(fileProperties()));

    QMenu * toolsMenu = manager->menu(Core::MENU_TOOLS);
    QMenu * menu = new QMenu("MPQ Viewer");

    menu->addAction(actionAdd);
    menu->addAction(actionExtract);
    menu->addAction(actionRename);
    menu->addAction(actionRemove);

    menu->addSeparator();

    menu->addAction(actionReopen);
    menu->addAction(actionCompact);
    menu->addAction(actionSetHashTableSize);
    menu->addAction(actionChangeAttributes);
    menu->addAction(actionVerifyArchive);
    menu->addAction(actionVerifyFiles);
    menu->addAction(actionArchiveProperties);
    menu->addAction(actionFileProperties);

    toolsMenu->addMenu(menu);
}

//Q_EXPORT_PLUGIN2(mpq_editor_factory, MPQEditorFactory)
Q_EXPORT_PLUGIN(MPQEditorPlugin)
