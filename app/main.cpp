#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include "mainwindow.h"
#include "pluginmanager.h"
#include <qmpqfileenginehandler.h>
#include <QDebug>

Q_IMPORT_PLUGIN(mpq_editor_factory)

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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(mpqeditor);

    QMPQFileEngineHandler h;
    MainWindow w;
//    w.installEventFilter(new EventFilter());
    w.show();
//    w.showMaximized();

    int code = app.exec();
    PluginManager::shutdown();
    return code;
}
