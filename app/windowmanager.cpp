#include "windowmanager.h"

#include <icore.h>

#include "mainwindow.h"

WindowManager::WindowManager(QObject *parent) :
    IWindowManager(parent)
{
}

bool WindowManager::open(const QString path)
{
    ICore * core = ICore::instance();
    MainWindow * window = qobject_cast<MainWindow *>(core->context()->mainWindow());
    window->open(path);
}

bool WindowManager::openInNewTab(const QString path)
{
    ICore * core = ICore::instance();
    MainWindow * window = qobject_cast<MainWindow *>(core->context()->mainWindow());
    window->newTab();
    window->open(path);
}

bool WindowManager::save(const QString path)
{
    ICore * core = ICore::instance();
    MainWindow * window = qobject_cast<MainWindow *>(core->context()->mainWindow());
    window->save(path);
}

bool WindowManager::close()
{
    ICore * core = ICore::instance();
    MainWindow * window = qobject_cast<MainWindow *>(core->context()->mainWindow());
    window->closeCurrentTab();
}
