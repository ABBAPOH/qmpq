#include "mainwindowplugin.h"

#include <icore.h>
#include <preferencesmanager.h>
#include <QtCore/QTimer>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>

#include "archivesuffixesmanager.h"
#include "createarchivedialog.h"
#include "mainwindow.h"
#include "mpqsettings.h"
#include "mpqextensionssettings.h"
#include "preferenceswidget.h"
#include "windowmanager.h"
#include "mpqarchiveerrorhandler.h"

#include "../QMPQFileEngine/qmpqarchiveex.h"
//#include "../QMPQFileEngine/mpqextensionmanager.h"

MainWindowPlugin::MainWindowPlugin(QObject *parent) :
    IPlugin(parent)
{
}

MainWindowPlugin::~MainWindowPlugin()
{
}

void MainWindowPlugin::initialize()
{
    ICore * core = ICore::instance();
    WindowManager * windowManager = new WindowManager(core);
    core->addObject(windowManager);
    core->pluginManager()->load();

    ArchiveSuffixesManager * manager = new ArchiveSuffixesManager;
//    QStringList suffixes;
//    suffixes << "mpq" << "w3x" << "w3m" << "s2ma" << "SC2Data" << "SC2Archive" << "SC2Assets"
//            << "SC2Replay" << "scx" << "w3n" << "snp" << "sv" << "hsv";

//    manager->addSuffixes(suffixes, "mpq:");
    manager->setObjectName("SuffixesManager");
    core->addObject(manager);

//    core->addObject(MPQExtensionManager::instance());

    PreferencesManager * preferencesManager = qobject_cast<PreferencesManager *>(core->getObject("PreferencesManager"));
    preferencesManager->addPreferencesPage(new MPQExtensionsPage(preferencesManager));
    preferencesManager->addPreferencesPage(new MPQSettingsPage(preferencesManager));

//    preferencesManager->loadSettings();

    MPQArchiveErrorHandler * handler = new MPQArchiveErrorHandler;
    handler->setObjectName("MPQArchiveErrorHandler");
    core->addObject(handler);

    m_preferencesWidget = new PreferencesWidget(preferencesManager);

    initializeMenus();

    MainWindow * w = new MainWindow();
    w->show();
    w->newTab();

    QTimer t;
    t.singleShot(10, this, SLOT(openFirstTab()));
}

void MainWindowPlugin::newArchive()
{
    QString file = ICore::instance()->fileManager()->getSaveFileName("", "MPQ Archives (*.mpq *.w3x *.w3m *.s2ma *.SC2Data *.SC2Archive *.SC2Assets *.SC2Replay *.scx *.w3n *.snp *.sv *.hsv)");
    if (!file.isEmpty()) {
        CreateArchiveDialog dialog;
        if (dialog.exec()) {
            int flags = 0, maxFiles = 1024;
            int version = dialog.archiveVersion();
            flags |= (version == 1) ? QMPQArchive::ArchiveV1 : 0;
            flags |= (version == 2) ? QMPQArchive::ArchiveV2 : 0;
            flags |= dialog.addAttributes() ? QMPQArchive::AddAttributes : 0;
            maxFiles = dialog.maxFiles();
            QMPQArchive arch;
            arch.create(file, maxFiles, QMPQArchive::CreateFlags(flags));
            arch.close();
            ICore::instance()->windowManager()->open(file);
        }
    }
}

void MainWindowPlugin::open()
{
    QStringList files = ICore::instance()->fileManager()->getOpenFileNames();
    if (!files.isEmpty()) {
        ICore::instance()->windowManager()->open(files.first());
    }
}

void MainWindowPlugin::save()
{
    ICore::instance()->windowManager()->save();
}

void MainWindowPlugin::save_As()
{
    QString saveName = ICore::instance()->fileManager()->getSaveFileName();
    if (!saveName.isEmpty()) {
        ICore::instance()->windowManager()->save(saveName);
    }
}

void MainWindowPlugin::openFirstTab()
{
    ICore::instance()->windowManager()->open(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
}

void MainWindowPlugin::newTab()
{
    ICore::instance()->windowManager()->openInNewTab(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
}

void MainWindowPlugin::closeTab()
{
    ICore::instance()->windowManager()->close();
}

void MainWindowPlugin::about()
{
    QMessageBox::about(0, tr("About QMPQ"),
             tr("<b>QMPQ 1.6</b> is a Qt-based program that allows to manipulate "
                "with Blizzard's MPQ-archives. "
                "Copyright 2010 Nevermore (N) aka ABBAPOH"));
}

void MainWindowPlugin::initializeMenus()
{
    ICore * core = ICore::instance();

    QMenu * fileMenu = core->actionManager()->createMenu(Core::MENU_FILE);
    fileMenu->setTitle(tr("File"));
    
    QAction * newArchiveAction = fileMenu->addAction(tr("New Archive"));
    connect(newArchiveAction, SIGNAL(triggered()), SLOT(newArchive()));
    newArchiveAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
    newArchiveAction->setIcon(QIcon(":/icons/images/filenew.png"));
    core->actionManager()->registerAction(newArchiveAction, "NEWARCH");

    QAction * openAction = fileMenu->addAction(tr("Open"));
    connect(openAction, SIGNAL(triggered()), SLOT(open()));
    openAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
    openAction->setIcon(QIcon(":/icons/images/fileopen.png"));
    core->actionManager()->registerAction(openAction, Core::ACTION_OPEN);

    QAction * saveAction = fileMenu->addAction(tr("Save"));
    connect(saveAction, SIGNAL(triggered()), SLOT(save()));
    saveAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
    saveAction->setIcon(QIcon(":/icons/images/save.png"));
    core->actionManager()->registerAction(saveAction, Core::ACTION_SAVE);

    QAction * save_AsAction = fileMenu->addAction(tr("Save As..."));
    connect(save_AsAction, SIGNAL(triggered()), SLOT(save_As()));
    save_AsAction->setIcon(QIcon(":/icons/images/save.png"));
//    save_AsAction->setShortcut(QApplication::translate("MainWindow", "", 0, QApplication::UnicodeUTF8));
    core->actionManager()->registerAction(save_AsAction, Core::ACTION_SAVE_AS);

    fileMenu->addSeparator();

    QAction * newTabAction = fileMenu->addAction(tr("New Tab"));
    connect(newTabAction, SIGNAL(triggered()), SLOT(newTab()));
    newTabAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", 0, QApplication::UnicodeUTF8));
    core->actionManager()->registerAction(newTabAction, Core::ACTION_NEW_TAB);

    QAction * closeTabAction = fileMenu->addAction(tr("Close Tab"));
    connect(closeTabAction, SIGNAL(triggered()), SLOT(closeTab()));
    closeTabAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
    core->actionManager()->registerAction(closeTabAction, Core::ACTION_CLOSE_TAB);

    QMenu * editMenu = core->actionManager()->createMenu(Core::MENU_EDIT);
    editMenu->setTitle(tr("Edit"));

    QAction * undoAction = editMenu->addAction(tr("Undo"));
    connect(undoAction, SIGNAL(triggered()), core->actionManager(), SLOT(undo()));
    undoAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
    undoAction->setIcon(QIcon(":/icons/images/undo.png"));
    core->actionManager()->registerAction(undoAction, Core::ACTION_UNDO);

    QAction * redoAction = editMenu->addAction(tr("Redo"));
    connect(redoAction, SIGNAL(triggered()), core->actionManager(), SLOT(redo()));
    redoAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", 0, QApplication::UnicodeUTF8));
    redoAction->setIcon(QIcon(":/icons/images/redo.png"));
    core->actionManager()->registerAction(redoAction, Core::ACTION_REDO);

    editMenu->addSeparator();

    QAction * cutAction = editMenu->addAction(tr("Cut"));
    connect(cutAction, SIGNAL(triggered()), core->actionManager(), SLOT(cut()));
    cutAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
    cutAction->setIcon(QIcon(":/icons/images/cut.png"));
    core->actionManager()->registerAction(cutAction, Core::ACTION_CUT);

    QAction * copyAction = editMenu->addAction(tr("Copy"));
    connect(copyAction, SIGNAL(triggered()), core->actionManager(), SLOT(copy()));
    copyAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
    copyAction->setIcon(QIcon(":/icons/images/copy.png"));
    core->actionManager()->registerAction(copyAction, Core::ACTION_COPY);

    QAction * pasteAction = editMenu->addAction(tr("Paste"));
    connect(pasteAction, SIGNAL(triggered()), core->actionManager(), SLOT(paste()));
    pasteAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
    pasteAction->setIcon(QIcon(":/icons/images/paste.png"));
    core->actionManager()->registerAction(pasteAction, Core::ACTION_PASTE);

    editMenu->addSeparator();

    QAction * preferencesAction = editMenu->addAction(tr("Preferences"));
    connect(preferencesAction, SIGNAL(triggered()), m_preferencesWidget, SLOT(show()));
    preferencesAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+,", 0, QApplication::UnicodeUTF8));
    core->actionManager()->registerAction(preferencesAction, Core::ACTION_PREFERENCES);

    QMenu * toolsMenu = core->actionManager()->menu(Core::MENU_TOOLS);
    toolsMenu->setTitle(tr("Tools"));


    QMenu * helpMenu = core->actionManager()->createMenu(Core::MENU_HELP);
    helpMenu->setTitle(tr("Help"));

    QAction * aboutAction = helpMenu->addAction(tr("About"));
    connect(aboutAction, SIGNAL(triggered()), SLOT(about()));
    core->actionManager()->registerAction(aboutAction, Core::ACTION_ABOUT);

    QAction * aboutQtAction = helpMenu->addAction(tr("About Qt"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    core->actionManager()->registerAction(aboutQtAction, Core::ACTION_ABOUT_QT);
}
