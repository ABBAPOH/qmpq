#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QHash>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QLineEdit>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QAction>

namespace Ui {
    class MainWindow
    {
    public:
        QLineEdit * lineEdit;
        QMenuBar * menuBar;
        QTabWidget * tabWidget;
        QToolBar * toolBar;
        QAction * actionBack;
        QAction * actionForward;
        QAction * actionUp;

        void setupUi(QMainWindow * parent)
        {
            parent->resize(920, 690);

            tabWidget = new QTabWidget(parent);
            tabWidget->setDocumentMode(true);
            tabWidget->setTabsClosable(true);

            menuBar = new QMenuBar(parent);
            parent->setMenuBar(menuBar);

            lineEdit = new QLineEdit(parent);

            actionBack = new QAction(parent);
            actionBack->setObjectName(QString::fromUtf8("actionBack"));
            actionBack->setEnabled(true);
            actionBack->setShortcut(QKeySequence::Back);
            QIcon icon2;
            icon2.addFile(QString::fromUtf8(":/icons/images/back.png"), QSize(), QIcon::Normal, QIcon::Off);
            actionBack->setIcon(icon2);

            actionForward = new QAction(parent);
            actionForward->setObjectName(QString::fromUtf8("actionForward"));
            actionForward->setEnabled(true);
            actionForward->setShortcut(QKeySequence::Forward);
            QIcon icon3;
            icon3.addFile(QString::fromUtf8(":/icons/images/forward.png"), QSize(), QIcon::Normal, QIcon::Off);
            actionForward->setIcon(icon3);

            actionUp = new QAction(parent);
            actionUp->setObjectName(QString::fromUtf8("actionUp"));
            actionUp->setEnabled(true);
            actionUp->setShortcut(QApplication::translate("MainWindow", "Ctrl+Up", 0, QApplication::UnicodeUTF8));
            QIcon icon4;
            icon4.addFile(QString::fromUtf8(":/icons/images/up.png"), QSize(), QIcon::Normal, QIcon::Off);
            actionUp->setIcon(icon4);

            toolBar = new QToolBar(parent);

            toolBar->addAction(actionBack);
            toolBar->addAction(actionForward);
            toolBar->addSeparator();
            toolBar->addAction(actionUp);
            toolBar->addWidget(lineEdit);

            parent->addToolBar(toolBar);
            parent->setUnifiedTitleAndToolBarOnMac(true);
        }
    };
}

class EditorView;
class EditorManager;
class QLabel;
class QLineEdit;

class QTabWidget;
class TabManager;
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    bool open(const QString &);
    bool save(const QString & path);
    void newTab();
    void closeCurrentTab();
    void back();
    void forward();
    void up();

private slots:
    void openManual(const QString &);
    void tabChanged(int index);
    void closeTab(int index);
//    void setCurrent(const QString &);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow * ui;
    TabManager * tabManager;
};

#endif // MAINWINDOW_H
