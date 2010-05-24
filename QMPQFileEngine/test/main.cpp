#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDirIterator>

#include "qmpqfileenginehandler.h"
#include "test.h"

#include <QDebug>

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);

    QString archiveName;
    if (argc == 2) {
        archiveName = argv[1];
    } else {
        archiveName = "./././(8)plainsofsnow5.w3m";
//        archiveName = "/Users/arch/thegame last!.w3x";
    }

    QMPQFileEngineHandler engine;

    Test t;
    t.testArchive(archiveName);
    t.testArchiveEx(archiveName);
    t.testEngine(archiveName);

//    return app.exec();
}

