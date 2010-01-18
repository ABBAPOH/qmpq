#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QHash>

namespace Ui {
    class MainWindow;
}

class IEditor;
class EditorManager;
class QLabel;
class QLineEdit;
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    EditorManager * core;
    QString currentFile;
//    QStringList currentFiles;
    QHash<QWidget*, QString> currentFiles;
//    QHash<QString, IEditor*> files;
    QLabel * title;
    QLineEdit * addressBar;
    QWidget * previousWidget;

    void initConnections();
    void showEditor(IEditor * editor);
    void connectAction(QAction * sender, const char * signal, QObject * receiver, const char * member);

public slots:
    void open(const QString & path = "");
    void closeCurrent();
    void newTab();
    void setAddress(const QString & path);

private slots:
    void tabChanged(int index);
    void closeTab(int index);
};

#endif // MAINWINDOW_H
