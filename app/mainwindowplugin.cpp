#include "mainwindowplugin.h"

#include <icore.h>
#include <QtCore/QTimer>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>

#include "windowmanager.h"
#include "mainwindow.h"
#include "createarchivedialog.h"
#include "archivesuffixesmanager.h"

#include "../QMPQFileEngine/qmpqarchive.h"

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

    initializeMenus();
    ArchiveSuffixesManager * manager = new ArchiveSuffixesManager;
    QStringList suffixes;
    suffixes << "mpq" << "w3x" << "w3m" << "s2ma" << "SC2Data" << "SC2Archive" << "SC2Assets"
            << "SC2Replay" << "scx" << "w3n" << "snp" << "sv" << "hsv";

    manager->addSuffixes(suffixes, "mpq:");
    manager->setObjectName("SuffixesManager");
    core->addObject(manager);

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
            flags |= (version == 1) ? QMPQArchive::CreateArchiveV1 : 0;
            flags |= (version == 2) ? QMPQArchive::CreateArchiveV2 : 0;
            flags |= dialog.addAttributes() ? QMPQArchive::CreateAttributes : 0;
            maxFiles = dialog.maxFiles();
            QMPQArchive arch;
            arch.newArchive(file, flags, maxFiles);
            arch.closeArchive();
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

    QMenu * fileMenu = core->actionManager()->createMenu("FILE");
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
    core->actionManager()->registerAction(openAction, "OPEN");

    QAction * saveAction = fileMenu->addAction(tr("Save"));
    connect(saveAction, SIGNAL(triggered()), SLOT(save()));
    saveAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
    saveAction->setIcon(QIcon(":/icons/images/save.png"));
    core->actionManager()->registerAction(saveAction, "SAVE");

    QAction * save_AsAction = fileMenu->addAction(tr("Save As..."));
    connect(save_AsAction, SIGNAL(triggered()), SLOT(save_As()));
    save_AsAction->setIcon(QIcon(":/icons/images/save.png"));
//    save_AsAction->setShortcut(QApplication::translate("MainWindow", "", 0, QApplication::UnicodeUTF8));
    core->actionManager()->registerAction(save_AsAction, "SAVE AS");

    fileMenu->addSeparator();

    QAction * newTabAction = fileMenu->addAction(tr("New Tab"));
    connect(newTabAction, SIGNAL(triggered()), SLOT(newTab()));
    newTabAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", 0, QApplication::UnicodeUTF8));
    core->actionManager()->registerAction(newTabAction, "NEW TAB");

    QAction * closeTabAction = fileMenu->addAction(tr("Close Tab"));
    connect(closeTabAction, SIGNAL(triggered()), SLOT(closeTab()));
    closeTabAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
    core->actionManager()->registerAction(closeTabAction, "CLOSE TAB");

    QMenu * editMenu = core->actionManager()->createMenu("EDIT");
    editMenu->setTitle(tr("Edit"));

    QAction * cutAction = editMenu->addAction(tr("Cut"));
    connect(cutAction, SIGNAL(triggered()), core->actionManager(), SLOT(cut()));
    cutAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
    cutAction->setIcon(QIcon(":/icons/images/cut.png"));
    core->actionManager()->registerAction(cutAction, "CUT");

    QAction * copyAction = editMenu->addAction(tr("Copy"));
    connect(copyAction, SIGNAL(triggered()), core->actionManager(), SLOT(copy()));
    copyAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
    copyAction->setIcon(QIcon(":/icons/images/copy.png"));
    core->actionManager()->registerAction(copyAction, "COPY");

    QAction * pasteAction = editMenu->addAction(tr("Paste"));
    connect(pasteAction, SIGNAL(triggered()), core->actionManager(), SLOT(paste()));
    pasteAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
    pasteAction->setIcon(QIcon(":/icons/images/paste.png"));
    core->actionManager()->registerAction(pasteAction, "PASTE");

    QMenu * helpMenu = core->actionManager()->createMenu("HELP");
    helpMenu->setTitle(tr("Help"));

    QAction * aboutAction = helpMenu->addAction(tr("About"));
    connect(aboutAction, SIGNAL(triggered()), SLOT(about()));
    core->actionManager()->registerAction(aboutAction, "ABOUT");

    QAction * aboutQtAction = helpMenu->addAction(tr("About Qt"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    core->actionManager()->registerAction(aboutQtAction, "ABOUT QT");
}
