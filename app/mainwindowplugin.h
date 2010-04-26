#ifndef MAINWINDOWPLUGIN_H
#define MAINWINDOWPLUGIN_H

#include <iplugin.h>

class MainWindowPlugin : public IPlugin
{
Q_OBJECT
public:
    explicit MainWindowPlugin(QObject *parent = 0);
    ~MainWindowPlugin();

    void initialize();
    void shutdown() {}
    QString name() { return "MainWindow"; }
    QString description() { return QString(); };

signals:

private slots:
    void open();
    void save();
    void save_As();
    void openFirstTab();
    void newTab();
    void closeTab();

    void about();
    void newArchive();

private:
    void initializeMenus();
};

#endif // MAINWINDOWPLUGIN_H