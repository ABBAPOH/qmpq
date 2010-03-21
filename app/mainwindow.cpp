#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui/QFileDialog>
#include <QtGui/QToolBar>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QDesktopServices>
#include <QtGui/QMessageBox>
#include <QDebug>

#include "editormanager.h"
#include "editorview.h"
#include "pluginmanager.h"
#include "../plugins/editorplugins/MPQEditor/mpqeditor.h"

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
    connect(ui->actionSave_As, SIGNAL(triggered()), SLOT(save_As()));

//  Window menu
    connect(ui->actionNew_Tab, SIGNAL(triggered()), ui->tabWidget, SLOT(newTab()));

//  Help menu
    connect(ui->actionAbout_QMPQ, SIGNAL(triggered()), SLOT(about()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(ui->tabWidget, SIGNAL(currentChanged(QWidget *)), SLOT(connectView(QWidget *)));
    connect(core, SIGNAL(openRequested(const QString &)), SLOT(open(const QString &)));
}

bool MainWindow::connectAction(QAction * sender, const char * signal, QObject * receiver, const char * member)
{
    if (!receiver || !sender)
        return false;
    if (receiver->metaObject()->indexOfSlot(member+1) == -1) {
//        qDebug() << "can't connect: class "<< receiver->metaObject()->className() << "has no slot" << member+1;
        sender->setEnabled(false);
        return false;
    } else {
        sender->setEnabled(true);
        connect(sender, signal, receiver, member);
        return true;
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

void MainWindow::save_As()
{
    QString saveFilter = m_editorView->saveFilter();//property("saveFilter").toString();
    saveFilter += ";;" + tr("AllFiles (*.*)");
    QString path = QFileDialog::getSaveFileName(m_editor, tr("Save As..."), m_editorView->property("path").toString(), saveFilter);
    if (path == "")
        return;
    m_editorView->save(path);
}

void MainWindow::closeCurrent()
{
    if (ui->actionSave->isEnabled()) {
        QMessageBox messageBox(this);
        messageBox.setText("File " + m_editorView->path() + " has been modified. Save changes?");
        messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Save);
        switch (messageBox.exec()) {
        case QMessageBox::Save: m_editorView->save(); break;
        case QMessageBox::Discard: break;
        case QMessageBox::Cancel: return;
        }
    }
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
            tr("<b>QMPQ 1.5c</b> a Qt-based program that allows to manipulate "
               "with Blizzard's MPQ-archives. "
               "Copyright 2009 Nevermore (N) aka ABBAPOH"));
}

void MainWindow::disconnectEditor(QWidget * editor)
{
    disconnect(ui->actionCut, 0, editor, 0);
    disconnect(ui->actionCopy, 0, editor, 0);
    disconnect(ui->actionPaste, 0, editor, 0);
    disconnect(ui->actionSelect_All, 0, editor, 0);
    disconnect(this, SLOT(setSavingEnabled(bool)));
}

void MainWindow::connectEditor(QWidget * editor)
{
    disconnectEditor(m_editor);
    m_editor = editor;

    connectAction(ui->actionCut, SIGNAL(triggered()), editor, SLOT(cut()));
    connectAction(ui->actionCopy, SIGNAL(triggered()), editor, SLOT(copy()));
    connectAction(ui->actionPaste, SIGNAL(triggered()), editor, SLOT(paste()));
    connectAction(ui->actionSelect_All, SIGNAL(triggered()), editor, SLOT(selectAll()));

    ui->actionSave->setEnabled(false);
    if (hasSignal(editor, SIGNAL(modificationChanged(bool)))) {
        ui->actionSave_As->setEnabled(true);
        connect(editor, SIGNAL(modificationChanged(bool)), SLOT(setSavingEnabled(bool)));
    } else {
        ui->actionSave_As->setEnabled(false);
    }
}

void MainWindow::connectView(QWidget * view)
{
    EditorView * editorView = qobject_cast<EditorView *>(view);
    Q_ASSERT(editorView);
    m_editorView = editorView;

    disconnectView(ui->tabWidget->previousWidget());

    connect(view, SIGNAL(pathChanged(const QString &)), this, SLOT(setAddress(const QString &)));
    connect(view, SIGNAL(centralWidgetChanged(QWidget *)), this, SLOT(connectEditor(QWidget *)));
    connect(addressBar, SIGNAL(textChanged(const QString &)), view, SLOT(setPath(const QString &)));
    connect(ui->actionBack, SIGNAL(triggered()), view, SLOT(back()));
    connect(ui->actionForward, SIGNAL(triggered()), view, SLOT(forward()));
    connect(ui->actionUp_one_level, SIGNAL(triggered()), view, SLOT(up()));
    connect(ui->actionSave, SIGNAL(triggered()), view, SLOT(save()));

    QWidget * centralWidget = editorView->centralWidget();
    connectEditor(centralWidget);

    setAddress(editorView->path()); // changes current url
}

void MainWindow::disconnectView(QWidget * view)
{
    disconnect(view, 0, addressBar, 0);
    disconnect(view, 0, this, 0);
    disconnect(addressBar, 0, view, 0);
    disconnect(ui->actionBack, 0, view, 0);
    disconnect(ui->actionForward, 0, view, 0);
    disconnect(ui->actionUp_one_level, 0, view, 0);
    disconnect(ui->actionSave, 0, view, 0);
}

bool MainWindow::hasSignal(QObject * object, const char * slot)
{
    return object->metaObject()->indexOfSignal(slot+1) != -1;
}

void MainWindow::setSavingEnabled(bool enable)
{
    ui->actionSave->setEnabled(enable);
}
