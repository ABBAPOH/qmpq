#include "test.h"

#include <QDir>
#include <QDebug>

Test::Test(QObject *parent) :
    QObject(parent)
{
    archive = new QMPQArchive(this);
    archiveEx = new QMPQArchiveEx(this);

    connect(archive, SIGNAL(error()), SLOT(archiveError()));
    connect(archive, SIGNAL(addFileProgressChanged(quint32,quint32,bool)), SLOT(addFileProgressChanged(quint32,quint32,bool)));
    connect(archive, SIGNAL(compactProgressChanged(QMPQArchive::CompactOperation,qint64,qint64)), SLOT(compactProgressChanged(QMPQArchive::CompactOperation,qint64,qint64)));
}

void Test::testArchive(const QString & archiveName)
{
    qDebug() << archive->open(archiveName);
    qDebug() << "number of files" << archive->filesCount();
    qDebug() << "hash table size" << archive->hashTableSize();
    qDebug() << "block table size" << archive->blockTableSize();
    qDebug() << "sector size" << archive->sectorSize();

    MPQFileInfo info = archive->fileInfo("(listfile)");
    qDebug() << "file name" << info.name();
    qDebug() << "file size" << info.fileSize();
    qDebug() << "comp size" << info.compressedSize();
    qDebug() << archive->read("(listfile)");

    qDebug() << "remove file"<< archive->remove("1.zip");
    qDebug() << "add file" << archive->add(QString("/Users/arch/1.zip"), "1.zip");

    MPQFileInfoIterator * iterator = archive->beginFileInfoList();
    while(iterator->hasNext()) {
         MPQFileInfo info = iterator->next();
         qDebug() << "file name" << info.name();

    }
    delete iterator;
//    qDebug() << "compacting" << archive->compact();
    qDebug() << "closing" << archive->close();
}

void Test::testArchiveEx(const QString & archiveName)
{
    qDebug() << archiveEx->open(archiveName);

    qDebug() << "closing" << archiveEx->close();
}

void Test::testEngine(const QString & archiveName)
{
    QString archiveFullName = QFileInfo(archiveName).absoluteFilePath();
    QDir d("mpq:" + archiveFullName);

    QFile f1("mpq:" + archiveFullName + "/(listfile)");
    QFile f2("mpq:" + archiveFullName + "/war3map.j");
    qDebug() << "f1 exists: " << f1.exists();
    //        qDebug() << f.remove();
    qDebug() << "f2 exists: " << f2.exists();
    //        qDebug() << f.rename("/Users/arch/3.mpq/portforward1");
    //        qDebug() << f.errorString();
    //        qDebug() << d.rename("portforward", "portforward1");
    //        f.open(QFile::ReadOnly);
    //        f.write("looooolqwertyuiopasdfghjklzxcvbnm,");
    //        f.close();
    qDebug() << "f2 contains ";
    f2.open(QFile::ReadOnly);
    qDebug() << f2.readAll();

    qDebug() << "entry list:";
    qDebug() << d.entryList(QDir::AllEntries);
}

void Test::archiveError()
{
    qDebug() << "Test::archiveError!!!";
}

void Test::addFileProgressChanged(quint32 a,quint32 b ,bool c)
{
    qDebug() << "Test::addFileProgressChanged" << a << b << c;
}

void Test::compactProgressChanged(QMPQArchive::CompactOperation a, qint64 b, qint64 c)
{
    qDebug() << "Test::compactProgressChanged" << a << b << c;
}
