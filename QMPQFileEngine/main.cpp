#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDirIterator>

#include "qmpqfileenginehandler.h"
//#include "qmpqarchive.h"
//#include "qmpqarchive2.h"
#include "test.h"

#include "sharedmpqarchive.h"

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);

    Test t;


//    QMPQFileEngineHandler engine;
//    QMPQArchive a;
//    a.openArchive("/Users/arch/Documents/qt4/QMPQFileEngine/QMPQFileEngine.app/Contents/MacOs/1.w3x");
//    qDebug() << a.extract("(listfile)", "/Users/arch");

//    SharedMPQArchive * a2 = SharedMPQArchive::instance("/Users/arch/Documents/qt4/QMPQFileEngine/QMPQFileEngine.app/Contents/MacOs/1.w3x");
//    SharedMPQArchive * a3 = SharedMPQArchive::instance("/Users/arch/Documents/qt4/QMPQFileEngine/QMPQFileEngine.app/Contents/MacOs/1.w3x");

//    qDebug () << "test";

//    QDir d("/Users/arch/3.mpq");

//    qDebug() <<QFile().exists("/Users/arch/Desktop/1.zip/entry3");
//    d.mkdir("lol");
//    qDebug() << d.entryList(QDir::AllEntries);
//    QFile f("/Users/arch/Documents/qt4/QMPQFileEngine/1.w3x/(listfile)");
//    QFile f("/Users/arch/3.mpq/portforward2");
//    qDebug() << f.exists();
//    qDebug() << f.remove();
//    qDebug() << f.exists();
//    qDebug() << f.rename("/Users/arch/3.mpq/portforward1");
//    qDebug() << f.errorString();
//    qDebug() << d.rename("portforward", "portforward1");
//    f.open(QFile::WriteOnly);
//    f.write("looooolqwertyuiopasdfghjklzxcvbnm,");
//    f.close();
//    f.open(QFile::ReadOnly);
//    qDebug() << f.read(200);
//    qDebug() << d.entryList();
//    QDirIterator i(d);
//    qDebug() <<i.next();
//    qDebug() <<i.next();
//    qDebug() <<i.next();
//    qDebug() <<i.next();
//    return app.exec();
}

