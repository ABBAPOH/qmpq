#include "sharedmpqarchive.h"

#include <QDebug>

SharedMPQArchive::SharedMPQArchive(const QString &file)
    : QMPQArchive()
{
    openArchive(file);
}

SharedMPQArchive::~SharedMPQArchive()
{
}
