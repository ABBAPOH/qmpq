#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include <QtCore/QTranslator>
#include <QtGui/QTreeView>

#include "mainwindow.h"
#include "pluginmanager.h"
#include "filemanager.h"
#include "mainwindowplugin.h"
#include "../QMPQFileEngine/qmpqarchivecache.h"
#include "../3rdParty/QDirModel/qdirmodel.h"

#include <qmpqfileenginehandler.h>
#include <QDebug>
#include <core.h>

Q_IMPORT_PLUGIN(MPQEditorPlugin)

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

    QMPQFileEngineHandler h;

    CoreImpl core;
    MainWindowPlugin plugin;
    plugin.initialize();

    int code = app.exec();
    return code;
}
