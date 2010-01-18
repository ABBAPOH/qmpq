#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui/QFileDialog>
#include <QVBoxLayout>
#include <QtGui/QToolBar>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QDebug>

#include "editormanager.h"
#include "ieditor.h"
#include "pluginmanager.h"
#include "../plugins/editorplugins/MPQEditor/mpqeditor.h"

#include "editorview.h"

#include <qmpqfileenginehandler.h> // fix lock!!!!

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    core(new EditorManager)
{
    ui->setupUi(this);
    title = new QLabel();
                 title->setAlignment(Qt::AlignCenter);
    title->setMinimumHeight(qApp->style()->pixelMetric(QStyle::PM_TabBarTabHSpace) - 1);

    setUnifiedTitleAndToolBarOnMac(true);
//    ui->toolBar->addAction(ui->actionBack);
//    ui->toolBar->addAction(ui->actionForward);
//    ui->toolBar->addSeparator();
//    ui->toolBar->addAction(ui->actionUp_one_level);
//    ui->toolBar->addSeparator();
    addressBar = new QLineEdit(this);
    ui->toolBar->addWidget(addressBar);
    initConnections();

    previousWidget = 0;
    newTab();
    setAddress("");
}

MainWindow::~MainWindow()
{
    delete ui;
    PluginManager::shutdown();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::initConnections()
{
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(open()));
    connect(ui->actionClose, SIGNAL(triggered()), SLOT(closeCurrent()));

    connect(ui->actionNew_Tab, SIGNAL(triggered()), SLOT(newTab()));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
    connect(core, SIGNAL(openRequested(const QString &)), SLOT(open(const QString &)));
}

void MainWindow::showEditor(IEditor *)
{

}

void MainWindow::connectAction(QAction * sender, const char * signal, QObject * receiver, const char * member)
{
    if (!receiver || !sender)
        return;
    if (receiver->metaObject()->indexOfSlot(member) == -1)
        sender->setEnabled(false);
    else {
        sender->setEnabled(true);
        connect(sender, signal, receiver, member);
    }
}

void MainWindow::open(const QString & path)
{
//    qDebug() << "open";
    QString fileName = path;
    if (fileName == "") {
        QMPQFileEngineHandler::setLocked(true);
        fileName = QFileDialog::getOpenFileName(this, tr("Select Archive File"));
        QMPQFileEngineHandler::setLocked(false);
    }
    if (fileName == "")
        return;

    title->setText(QFileInfo(fileName).fileName());
    setAddress(fileName);
//    openEditor(fileName);
}

void MainWindow::closeCurrent()
{
//    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
//    core->close(currentFile);
//    mpq_editor->close();
    closeTab(ui->tabWidget->currentIndex());
}

void MainWindow::newTab()
{
    EditorView * view = new EditorView(this);
    int index = ui->tabWidget->addTab(view, "");
    ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::setAddress(const QString & path)
{
    addressBar->setText(path);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QFileInfo(path).fileName());
}

void MainWindow::tabChanged(int index)
{
    QWidget * widget = ui->tabWidget->widget(index);
    Q_ASSERT(widget);
    EditorView * view = qobject_cast<EditorView *>(widget);
    Q_ASSERT(view);
//    qDebug() << index << currentFiles.value(widget);
    if (previousWidget)
        disconnect(addressBar, 0, previousWidget, 0);
    setAddress(view->currentUrl());
    connectAction(ui->actionSave, SIGNAL(triggered()), widget, SLOT(save()));
    connect(ui->actionUp_one_level, SIGNAL(triggered()), view, SLOT(up()));
    connect(addressBar, SIGNAL(textChanged(const QString &)), view, SLOT(setUrl(const QString &)));
    connect(view, SIGNAL(currentUrlChanged(const QString &)), this, SLOT(setAddress(const QString &)));

    previousWidget = widget;
//    bool enabled;
//    enabled = connect(ui->actionSave, SIGNAL(triggered()), widget, SLOT(save()));
//    ui->actionSave->setEnabled(enabled);
}

void MainWindow::closeTab(int index)
{
    if (ui->tabWidget->count() == 1) // do not allow to close last tab
        return;

    QWidget * widget = ui->tabWidget->widget(index);
    EditorView * view = qobject_cast<EditorView *>(widget);
//    QString file = currentFiles.value(widget);
    ui->tabWidget->removeTab(index);
    delete view;
//    core->close(file);
//    mpq_editor->close();
}


