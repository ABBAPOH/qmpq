#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QToolBar>
#include <QtGui/QMenuBar>
#include <QDebug>

//#include <qmpqfileenginehandler.h> // fix lock!!!!

#include <icore.h>
#include <ieditor.h>

#include "editormanager.h"
#include "tabcontext.h"
#include "tabmanager.h"
#include "historymanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabManager(new TabManager(this))
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);

    QMenu * fileMenu = ICore::instance()->actionManager()->menu("FILE");
    QMenu * editMenu = ICore::instance()->actionManager()->menu("EDIT");
    QMenu * helpMenu = ICore::instance()->actionManager()->menu("HELP");
    menuBar()->addAction(fileMenu->menuAction());
    menuBar()->addAction(editMenu->menuAction());
    menuBar()->addAction(helpMenu->menuAction());

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), tabManager, SLOT(changeCurrent(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)), Qt::QueuedConnection);
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    connect(ui->lineEdit, SIGNAL(textEdited(QString)), SLOT(openManual(QString)));

    connect(ui->actionBack, SIGNAL(triggered()), SLOT(back()));
    connect(ui->actionForward, SIGNAL(triggered()), SLOT(forward()));
    connect(ui->actionUp, SIGNAL(triggered()), SLOT(up()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::changeEvent(QEvent *e)
{
//    QMainWindow::changeEvent(e);
//    switch (e->type()) {
//    case QEvent::LanguageChange:
//        ui->retranslateUi(this);
//        break;
//    default:
//        break;
//    }
}

bool MainWindow::open(const QString & path)
{
    QFileInfo info(path);
    if (!(path == ""  || info.exists()))
        return false;
    IEditor * editor = tabManager->context()->editorManager()->open(path);
    if (!editor)
        return false;
    ui->lineEdit->setText(QDir::toNativeSeparators(path));
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), info.fileName());

    tabManager->context()->setEditor(editor);
    tabManager->context()->historyManager()->setPath(path);
    ICore::instance()->editorFactoryManager()->setCurrentEditor(editor);
//    editor->widget()->setFocus();

    return true;
}

bool MainWindow::save(const QString & path)
{
    IEditor * editor = ICore::instance()->editorFactoryManager()->currentEditor();
    if (editor) {
        editor->save(path);
    }
    return true;
}

void MainWindow::newTab()
{
    TabContext * context = new TabContext();
    int index = ui->tabWidget->addTab(context->widget(), "");
    tabManager->addContext(index, context);
    ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::closeCurrentTab()
{
    closeTab(ui->tabWidget->currentIndex());
}

void MainWindow::back()
{
    TabContext * context = tabManager->context();
    HistoryManager * manager = context->historyManager();
    manager->back();
    open(manager->path());
}

void MainWindow::forward()
{
    TabContext * context = tabManager->context();
    HistoryManager * manager = context->historyManager();
    manager->forward();
    open(manager->path());
}

void MainWindow::up()
{
    TabContext * context = tabManager->context();
    HistoryManager * manager = context->historyManager();
    manager->up();
    open(manager->path());
}

void MainWindow::openManual(const QString & path)
{
    ICore::instance()->windowManager()->open(QDir::fromNativeSeparators(path));
}

void MainWindow::tabChanged(int index)
{
//    qDebug() << "MainWindow::tabChanged" << index;
    TabContext * context = tabManager->context();
    IEditor * editor = context->editor();
    if (!editor)
        return;
    ICore::instance()->editorFactoryManager()->setCurrentEditor(editor);
    ui->lineEdit->setText(QDir::toNativeSeparators(context->editor()->currentFile()));
}

void MainWindow::closeTab(int index)
{
//    qDebug() << "MainWindow::closeTab" << index;
    if (ui->tabWidget->count() == 1)
        return;
    TabContext * context = tabManager->context(index);
    IEditor * editor = context->editor();

    if (context->editorManager()->close(editor->currentFile())) {
        tabManager->remove(index);
        delete context;
        ui->tabWidget->removeTab(index);
    }
//    context->editor()->close();
}
