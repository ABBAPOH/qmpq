#include "qmpqfileenginehandler.h"

#include "qmpqfileengine.h"

#include <QtCore/QDebug>

bool QMPQFileEngineHandler::lock = false;

QMPQFileEngineHandler::QMPQFileEngineHandler()
{
//    qDebug() << "QMPQFileEngineHandler::QMPQFileEngineHandler";
}

QAbstractFileEngine *QMPQFileEngineHandler::create(const QString &fileName) const
{
    if (lock)
        return 0;
    // ZipEngineHandler returns a ZipEngine for all .zip files
//    return fileName.toLower().contains(".mpq") ? new QMPQFileEngine(fileName) : 0;
    return QRegExp(".*(\\.mpq|\\.w3x).*").exactMatch(fileName.toLower()) ? new QMPQFileEngine(fileName) : 0;
}
