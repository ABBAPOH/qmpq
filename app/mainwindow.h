#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QHash>

namespace Ui {
    class MainWindow;
}

class EditorView;
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
    QWidget * m_editor;
    EditorView * m_editorView;
    QString m_saveExtensions;

    void initActions();
    void initConnections();
    bool connectAction(QAction * sender, const char * signal, QObject * receiver, const char * member);
    void disconnectView(QWidget * view);
    void disconnectEditor(QWidget * editor);

    bool hasSignal(QObject * obj, const char * slot);
    bool hasSlot(QObject * obj, const char * slot);
//    void parseFormats(QStringList formats);

public slots:
    void open(const QString & path = "");
    void save_As();
    void closeCurrent();
    void setAddress(const QString & path);
    void about();

private slots:
    void connectView(QWidget * view);
    void connectEditor(QWidget * editor);
    void setSavingEnabled(bool enable);
};

#endif // MAINWINDOW_H
