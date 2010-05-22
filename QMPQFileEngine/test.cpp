#include "test.h"

#include <QDebug>

Test::Test(QObject *parent) :
    QObject(parent)
{
    archive = new QMPQArchive(this);
    archiveEx = new QMPQArchiveEx(this);

    connect(archive, SIGNAL(error()), SLOT(archiveError()));
    connect(archive, SIGNAL(addFileProgressChanged(quint32,quint32,bool)), SLOT(addFileProgressChanged(quint32,quint32,bool)));
    connect(archive, SIGNAL(compactProgressChanged(QMPQArchive::CompactOperation,qint64,qint64)), SLOT(compactProgressChanged(QMPQArchive::CompactOperation,qint64,qint64)));
//    testArchive();
    testArchiveEx();
}

void Test::testArchive()
{
    qDebug() << archive->openArchive("/Users/arch/(8)plainsofsnow5.w3m");
    qDebug() << "number of files" << archive->filesCount();
    qDebug() << "hash table size" << archive->hashTableSize();
    qDebug() << "block table size" << archive->blockTableSize();
    qDebug() << "sector size" << archive->sectorSize();

    MPQFileInfo info = archive->fileInfo("(listfile)");
    qDebug() << "file name" << info.name();
    qDebug() << "file size" << info.fileSize();
    qDebug() << "comp size" << info.compressedSize();
    qDebug() << archive->readFile("(listfile)");

    qDebug() << "remove file"<< archive->removeFile("1.zip");
    qDebug() << "add file" << archive->addFile(QString("/Users/arch/1.zip"), "1.zip");

    MPQFileInfoIterator * iter = archive->beginFileInfoList();
    while(iter->hasNext()) {
         MPQFileInfo info = iter->next();
         qDebug() << "file name" << info.name();

    }
//    qDebug() << "compacting" << archive->compact();
    qDebug() << "closing" << archive->closeArchive();
}

void Test::testArchiveEx()
{
    qDebug() << archiveEx->openArchive("/Users/arch/thegame last!.w3x");

    qDebug() << "closing" << archiveEx->closeArchive();
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
