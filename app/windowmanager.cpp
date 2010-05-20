#include "windowmanager.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QUuid>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>

#include <icore.h>

#include "mainwindow.h"
#include "archivesuffixesmanager.h"

#include <QDebug>

WindowManager::WindowManager(QObject *parent) :
    IWindowManager(parent)
{
}

bool WindowManager::open(const QString path)
{
    ICore * core = ICore::instance();
    MainWindow * window = qobject_cast<MainWindow *>(core->context()->mainWindow());
    Q_ASSERT(window);
    ArchiveSuffixesManager * manager = qobject_cast<ArchiveSuffixesManager *>(core->getObject("SuffixesManager"));
    Q_ASSERT(manager);
    QString prefix = manager->mapFromPath(path);

    if (window) {
        if (prefix != "")
            return window->open(prefix + path);
        else
            return window->open(path);
    }
    return false;
}

bool WindowManager::openInNewTab(const QString path)
{
    ICore * core = ICore::instance();
    MainWindow * window = qobject_cast<MainWindow *>(core->context()->mainWindow());
    window->newTab();
    return window->open(path);

}

bool WindowManager::openInNewWindow(const QString path)
{
    MainWindow * window = new MainWindow();
    window->show();
    window->newTab();
    return window->open(path);
}

bool WindowManager::openExternally(const QString path)
{
    QFile file(path);
    QString temp = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
//    QUuid uid = QUuid::createUuid();
    QString targetPath = temp + '/' + /*uid.toString() + '/' + */QFileInfo(path).fileName();
    file.copy(targetPath);
    return QDesktopServices::openUrl(QUrl::fromLocalFile(targetPath));
}

bool WindowManager::save(const QString path)
{
    ICore * core = ICore::instance();
    MainWindow * window = qobject_cast<MainWindow *>(core->context()->mainWindow());
    return window->save(path);
}

bool WindowManager::close()
{
    ICore * core = ICore::instance();
    MainWindow * window = qobject_cast<MainWindow *>(core->context()->mainWindow());
    window->closeCurrentTab();
    return true;
}
