#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include "mainwindow.h"
#include <qmpqfileenginehandler.h>
#include <QDebug>

Q_IMPORT_PLUGIN(mpq_editor_factory)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(mpqeditor);

    QMPQFileEngineHandler h;

    MainWindow w;
    w.show();
//    w.showMaximized();

    return app.exec();
}
