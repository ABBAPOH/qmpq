/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Apr 1 16:27:09 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "tabwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionClose;
    QAction *actionBack;
    QAction *actionForward;
    QAction *actionUp_one_level;
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionPaste;
    QAction *actionSelect_All;
    QAction *actionNew_Tab;
    QAction *actionAbout_QMPQ;
    QAction *actionAbout_Qt;
    QAction *actionSave_As;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    TabWidget *tabWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuWindow;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/images/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon1);
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionBack = new QAction(MainWindow);
        actionBack->setObjectName(QString::fromUtf8("actionBack"));
        actionBack->setEnabled(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/images/prev.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBack->setIcon(icon2);
        actionForward = new QAction(MainWindow);
        actionForward->setObjectName(QString::fromUtf8("actionForward"));
        actionForward->setEnabled(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/images/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionForward->setIcon(icon3);
        actionUp_one_level = new QAction(MainWindow);
        actionUp_one_level->setObjectName(QString::fromUtf8("actionUp_one_level"));
        actionUp_one_level->setEnabled(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/images/up.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUp_one_level->setIcon(icon4);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionCopy->setEnabled(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/images/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon5);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/images/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon6);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/images/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon7);
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        actionNew_Tab = new QAction(MainWindow);
        actionNew_Tab->setObjectName(QString::fromUtf8("actionNew_Tab"));
        actionAbout_QMPQ = new QAction(MainWindow);
        actionAbout_QMPQ->setObjectName(QString::fromUtf8("actionAbout_QMPQ"));
        actionAbout_Qt = new QAction(MainWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new TabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setDocumentMode(true);
        tabWidget->setTabsClosable(true);
        tabWidget->setMovable(true);

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuWindow = new QMenu(menubar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuWindow->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);
        menuWindow->addAction(actionNew_Tab);
        menuHelp->addAction(actionAbout_QMPQ);
        menuHelp->addAction(actionAbout_Qt);
        toolBar->addAction(actionBack);
        toolBar->addAction(actionForward);
        toolBar->addSeparator();
        toolBar->addAction(actionUp_one_level);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QMPQ", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionClose->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        actionBack->setText(QApplication::translate("MainWindow", "Back", 0, QApplication::UnicodeUTF8));
        actionBack->setShortcut(QApplication::translate("MainWindow", "Ctrl+[", 0, QApplication::UnicodeUTF8));
        actionForward->setText(QApplication::translate("MainWindow", "Forward", 0, QApplication::UnicodeUTF8));
        actionForward->setShortcut(QApplication::translate("MainWindow", "Ctrl+], Return", 0, QApplication::UnicodeUTF8));
        actionUp_one_level->setText(QApplication::translate("MainWindow", "Up one level", 0, QApplication::UnicodeUTF8));
        actionUp_one_level->setShortcut(QApplication::translate("MainWindow", "Ctrl+Up", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", 0, QApplication::UnicodeUTF8));
        actionCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("MainWindow", "Cut", 0, QApplication::UnicodeUTF8));
        actionCut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("MainWindow", "Paste", 0, QApplication::UnicodeUTF8));
        actionPaste->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setText(QApplication::translate("MainWindow", "Select All", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionNew_Tab->setText(QApplication::translate("MainWindow", "New Tab", 0, QApplication::UnicodeUTF8));
        actionNew_Tab->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", 0, QApplication::UnicodeUTF8));
        actionAbout_QMPQ->setText(QApplication::translate("MainWindow", "About QMPQ", 0, QApplication::UnicodeUTF8));
        actionAbout_Qt->setText(QApplication::translate("MainWindow", "About Qt", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As...", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        menuWindow->setTitle(QApplication::translate("MainWindow", "Window", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
