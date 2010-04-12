#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include "mainwindow.h"
#include "pluginmanager.h"
#include <qmpqfileenginehandler.h>
#include <QDebug>
#include <core.h>
#include "filemanager.h"
#include "mainwindowplugin.h"

Q_IMPORT_PLUGIN(MPQEditorPlugin)

class EventFilter : public QObject
{
    bool eventFilter(QObject * obj, QEvent * event)
    {
        int type = event->type();
        if (type!=12 && type !=129)
            qDebug() << event->type();
        return QObject::eventFilter(obj, event);
    }
};

#include "historymanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("QMPQ");
    app.setOrganizationName("QMPQ");
    Q_INIT_RESOURCE(mpqeditor);

    Core core;
    MainWindowPlugin plugin;
    plugin.initialize();
//    app.processEvents();

    QMPQFileEngineHandler h;
//    MainWindow * w = new MainWindow();
//    w->show();

    int code = app.exec();
//    delete w;
//    PluginManager::shutdown();
    return code;
}
