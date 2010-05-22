#include "sharedmpqarchive.h"

#include <QDebug>

SharedMPQArchive::SharedMPQArchive(const QString &file)
    : QMPQArchiveEx()
{
    openArchive(file);
}

SharedMPQArchive::~SharedMPQArchive()
{
}
