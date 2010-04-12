#include "mainwindowplugin.h"

#include <icore.h>
#include <QtCore/QTimer>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

#include "windowmanager.h"
#include "mainwindow.h"

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

    MainWindow * w = new MainWindow();
    w->show();
    w->newTab();

    QTimer t;
    t.singleShot(0, this, SLOT(openFirstTab()));
}

void MainWindowPlugin::open()
{
    QStringList files = ICore::instance()->fileManager()->getOpenFileNames();
    ICore::instance()->windowManager()->open(files.first());
}

void MainWindowPlugin::save()
{
    ICore::instance()->windowManager()->save();
}

void MainWindowPlugin::save_As()
{
    QString saveName = ICore::instance()->fileManager()->getSaveFileName();
    ICore::instance()->windowManager()->save(saveName);
}

void MainWindowPlugin::openFirstTab()
{
    ICore::instance()->windowManager()->open("/Users/arch");
}

void MainWindowPlugin::newTab()
{
    ICore::instance()->windowManager()->openInNewTab("/Users/arch");
}

void MainWindowPlugin::closeTab()
{
    ICore::instance()->windowManager()->close();
}

void MainWindowPlugin::about()
{
    QMessageBox::about(0, tr("About QMPQ"),
             tr("<b>QMPQ 1.6</b> a Qt-based program that allows to manipulate "
                "with Blizzard's MPQ-archives. "
                "Copyright 2009 Nevermore (N) aka ABBAPOH"));
}

void MainWindowPlugin::initializeMenus()
{
    ICore * core = ICore::instance();

    QMenu * fileMenu = core->actionManager()->createMenu("FILE");
    fileMenu->setTitle("File");
    
    QAction * openAction = fileMenu->addAction("Open");
    connect(openAction, SIGNAL(triggered()), SLOT(open()));
    openAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
    openAction->setIcon(QIcon(":/icons/images/copy.png"));
    core->actionManager()->registerAction(openAction, "OPEN");

    QAction * saveAction = fileMenu->addAction("Save");
    connect(saveAction, SIGNAL(triggered()), SLOT(save()));
    saveAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
    saveAction->setIcon(QIcon(":/icons/images/save.png"));
    core->actionManager()->registerAction(saveAction, "SAVE");

    QAction * save_AsAction = fileMenu->addAction("Save As...");
    connect(save_AsAction, SIGNAL(triggered()), SLOT(save_As()));
    save_AsAction->setIcon(QIcon(":/icons/images/save.png"));
//    save_AsAction->setShortcut(QApplication::translate("MainWindow", "", 0, QApplication::UnicodeUTF8));
    core->actionManager()->registerAction(save_AsAction, "SAVE AS");

    fileMenu->addSeparator();

    QAction * newTabAction = fileMenu->addAction("New Tab");
    connect(newTabAction, SIGNAL(triggered()), SLOT(newTab()));
    newTabAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", 0, QApplication::UnicodeUTF8));
    core->actionManager()->registerAction(newTabAction, "NEW TAB");

    QAction * closeTabAction = fileMenu->addAction("Close Tab");
    connect(closeTabAction, SIGNAL(triggered()), SLOT(closeTab()));
    closeTabAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
    core->actionManager()->registerAction(closeTabAction, "CLOSE TAB");

    QMenu * editMenu = core->actionManager()->createMenu("EDIT");
    editMenu->setTitle("Edit");

    QAction * cutAction = editMenu->addAction("Cut");
    connect(cutAction, SIGNAL(triggered()), core->actionManager(), SLOT(cut()));
    cutAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
    cutAction->setIcon(QIcon(":/icons/images/cut.png"));
    core->actionManager()->registerAction(cutAction, "CUT");

    QAction * copyAction = editMenu->addAction("Copy");
    connect(copyAction, SIGNAL(triggered()), core->actionManager(), SLOT(copy()));
    copyAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
    copyAction->setIcon(QIcon(":/icons/images/copy.png"));
    core->actionManager()->registerAction(copyAction, "COPY");

    QAction * pasteAction = editMenu->addAction("Paste");
    connect(pasteAction, SIGNAL(triggered()), core->actionManager(), SLOT(paste()));
    pasteAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
    pasteAction->setIcon(QIcon(":/icons/images/paste.png"));
    core->actionManager()->registerAction(pasteAction, "PASTE");

    QMenu * helpMenu = core->actionManager()->createMenu("HELP");
    helpMenu->setTitle("Help");

    QAction * aboutAction = helpMenu->addAction("About");
    connect(aboutAction, SIGNAL(triggered()), SLOT(about()));
    core->actionManager()->registerAction(aboutAction, "ABOUT");

    QAction * aboutQtAction = helpMenu->addAction("About Qt");
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    core->actionManager()->registerAction(aboutQtAction, "ABOUT QT");
}
