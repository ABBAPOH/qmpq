#include "sharedmpqarchive.h"

#include "QTimer"

#include <QDebug>

SharedMPQArchive::SharedMPQArchive(const QString &file)
    : QMPQArchive()
{
    openArchive(file);
}

SharedMPQArchive::~SharedMPQArchive()
{
}
