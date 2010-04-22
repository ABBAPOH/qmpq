#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include <QtCore/QTranslator>
#include "mainwindow.h"
#include "pluginmanager.h"
#include "filemanager.h"
#include "mainwindowplugin.h"

#include <qmpqfileenginehandler.h>
#include <QDebug>
#include <core.h>

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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("QMPQ");
    app.setOrganizationName("QMPQ");
    Q_INIT_RESOURCE(mpqeditor);

    QString locale = QLocale::system().name();

    QTranslator translator, qtTranslator;
    translator.load(QString("qmpq_") + locale);
    qtTranslator.load(QString("qt_") + locale);
    app.installTranslator(&translator);
    app.installTranslator(&qtTranslator);

    Core core;
    MainWindowPlugin plugin;
    plugin.initialize();

    QMPQFileEngineHandler h;

    int code = app.exec();
    return code;
}
