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

    QMPQArchiveCache::init();

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

//    QDirModel model("mpq:/Users/arch/TheGame LAST!.w3x");
//    QDirModel model(":/");
//    QDirModel model("");
//    QModelIndex index = model.index("/Applications");
//    QModelIndex index = model.index("mpq:/Users/arch/TheGame LAST!.w3x/ReplaceableTextures/CommandButtons/BTNBowEluna.blp");
//    QModelIndex index = model.index("mpq:/Users/arch/TheGame LAST!.w3x");
//    qDebug() << index;
//    qDebug() << model.filePath(index);
//    QTreeView view;
//    view.setModel(&model);
//    view.show();
//
//    QDir d("mpq:/Users/arch/TheGame LAST!.w3x");
//    qDebug() << d.entryList();

    int code = app.exec();
    return code;
}
