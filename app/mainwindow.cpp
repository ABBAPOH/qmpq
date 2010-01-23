#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui/QFileDialog>
#include <QVBoxLayout>
#include <QtGui/QToolBar>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QDesktopServices>
#include <QtGui/QMessageBox>
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
    addressBar = new QLineEdit(this);
    ui->toolBar->addWidget(addressBar);
    initConnections();

    previousWidget = 0;
    ui->tabWidget->newTab();
}

MainWindow::~MainWindow()
{
    delete ui;
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
//  File menu
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(open()));
    connect(ui->actionClose, SIGNAL(triggered()), SLOT(closeCurrent()));

//  Window menu
    connect(ui->actionNew_Tab, SIGNAL(triggered()), ui->tabWidget, SLOT(newTab()));

//  Help menu
    connect(ui->actionAbout_QMPQ, SIGNAL(triggered()), SLOT(about()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(ui->tabWidget, SIGNAL(currentChanged(QWidget *)), SLOT(connectView(QWidget *)));
    connect(core, SIGNAL(openRequested(const QString &)), SLOT(open(const QString &)));
}

void MainWindow::connectAction(QAction * sender, const char * signal, QObject * receiver, const char * member)
{
    if (!receiver || !sender)
        return;
    if (receiver->metaObject()->indexOfSlot(member+1) == -1) {
//        qDebug() << "can't connect: class "<< receiver->metaObject()->className() << "has no slot" << member+1;
        sender->setEnabled(false);
    } else {
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
}

void MainWindow::closeCurrent()
{
    ui->tabWidget->closeTab(ui->tabWidget->currentIndex());
}

void MainWindow::setAddress(const QString & path)
{
//    qDebug() << "MainWindow::setAddress" << path;
    addressBar->setText(QDir::toNativeSeparators(path));
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QFileInfo(path).fileName());
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About QMPQ"),
            tr("<b>QMPQ 1.3с</b> a Qt-based program that allows to manipulate "
               "with Blizzard's MPQ-archives. "
               "Copyright 2009 Nevermore (N) aka ABBAPOH"));
}

void MainWindow::disconnectEditor(QWidget * editor)
{
    disconnect(ui->actionSave, 0, editor, 0);
    disconnect(ui->actionCut, 0, editor, 0);
    disconnect(ui->actionCopy, 0, editor, 0);
    disconnect(ui->actionPaste, 0, editor, 0);
    disconnect(ui->actionSelect_All, 0, editor, 0);
}

void MainWindow::connectEditor(QWidget * editor)
{
    disconnectEditor(m_editor);
    m_editor = editor;
    connectAction(ui->actionSave, SIGNAL(triggered()), editor, SLOT(save()));
    connectAction(ui->actionCut, SIGNAL(triggered()), editor, SLOT(cut()));
    connectAction(ui->actionCopy, SIGNAL(triggered()), editor, SLOT(copy()));
    connectAction(ui->actionPaste, SIGNAL(triggered()), editor, SLOT(paste()));
    connectAction(ui->actionSelect_All, SIGNAL(triggered()), editor, SLOT(selectAll()));
}

void MainWindow::connectView(QWidget * view)
{
    disconnectView(ui->tabWidget->previousWidget());
    connect(view, SIGNAL(currentUrlChanged(const QString &)), this, SLOT(setAddress(const QString &)));
    connect(view, SIGNAL(centralWidgetChanged(QWidget *)), this, SLOT(connectEditor(QWidget *)));
    connect(ui->actionBack, SIGNAL(triggered()), view, SLOT(back()));
    connect(ui->actionForward, SIGNAL(triggered()), view, SLOT(forward()));
    connect(ui->actionUp_one_level, SIGNAL(triggered()), view, SLOT(up()));
    connect(addressBar, SIGNAL(textChanged(const QString &)), view, SLOT(setUrl(const QString &)));

    EditorView * editorView = qobject_cast<EditorView *>(view);
    Q_ASSERT(editorView);
    connectEditor(editorView->centralWidget());
}

void MainWindow::disconnectView(QWidget * view)
{
    disconnect(addressBar, 0, view, 0);
    disconnect(ui->actionBack, 0, view, 0);
    disconnect(ui->actionForward, 0, view, 0);
    disconnect(ui->actionUp_one_level, 0, view, 0);
}
