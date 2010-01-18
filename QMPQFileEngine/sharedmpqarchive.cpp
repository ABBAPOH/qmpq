#include "sharedmpqarchive.h"

#include "QTimer"

#include <QDebug>

SharedMPQArchive::SharedMPQArchive(const QString &file)
    : QMPQArchive(), refcnt(1)
{
    openArchive(file);
}

SharedMPQArchive::~SharedMPQArchive()
{
//    qDebug() << "SharedMPQArchive::~SharedMPQArchive" << file();
}


SharedMPQArchive::MPQsInUse& SharedMPQArchive::mpqsInUse()
{
    static MPQsInUse mpqs;
//    if(!qZipsInUseInit)
//    {
//            qZipsInUseInit = true;
//            qAddPostRoutine(cleanZipsInUse);
//    }
    return mpqs;
}

void SharedMPQArchive::clearCache()
{
    if (!refcnt) {
        qDebug() << "SharedMPQArchive::clearCache" << file();
        SharedMPQArchive::mpqsInUse().remove(file());
        delete this;
    }
}

SharedMPQArchive* SharedMPQArchive::instance(const QString& file)
{
    SharedMPQArchive* archive = 0;

    QWriteLocker locker(&SharedMPQArchive::mpqsInUse().lock);
    archive = SharedMPQArchive::mpqsInUse().value(file);
    if (archive) {
            archive->refcnt.ref();
    } else {
            archive = new SharedMPQArchive(file);
//            if(zipReader->status() == QZipReader::NoError)
//            {
//                    zipReader->updateLocations();
                    SharedMPQArchive::mpqsInUse().insert(file, archive);
//            }
    }
//    qDebug() << "SharedMPQArchive::instance" << archive->refcnt;
    return archive;
}

void SharedMPQArchive::releaseInstance(const QString& file)
{
    if (file.isEmpty())
        return;
    QWriteLocker locker(&SharedMPQArchive::mpqsInUse().lock);
    SharedMPQArchive* archive = SharedMPQArchive::mpqsInUse().value(file);
//    qDebug() << "releaseInstance::releaseInstance" << archive->refcnt;
    if (archive && archive->refcnt.deref() == 0)
    {
//        archive->deleteLater();
//        QTimer::singleShot(10, archive, SLOT(clearCache()));
//        QMetaObject::invokeMethod(archive, "clearCache", Qt::QueuedConnection);
        archive->clearCache();
//        delete archive;
    }
}
