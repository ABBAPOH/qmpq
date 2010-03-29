#ifndef QMPQFILEENGINE_P_H
#define QMPQFILEENGINE_P_H

#include <QtCore/QString>
#include "sharedmpqarchive.h"

class QMPQFileEnginePrivate
{
    Q_DECLARE_PUBLIC(QMPQFileEngine)

    QString filePath;
    QString fileName;
    QString baseName;
    QString archiveFilePath;
    QString innerPath;
    SharedMPQArchive * archive;
//    QBuffer buffer;
    QIODevice::OpenMode openMode;
    uint mappedCnt;
    qint64 offset;
    QByteArray fileData;
    bool isCreated;

    QString getArchiveFilePath(const QString & path);

public:
    QMPQFileEnginePrivate(QMPQFileEngine * q)
        : archive(0), openMode(QIODevice::NotOpen), offset(0), isCreated(false)
    {
        q_ptr = q;
    }
protected:
    QMPQFileEngine *q_ptr;
};

#endif // QMPQFILEENGINE_P_H
