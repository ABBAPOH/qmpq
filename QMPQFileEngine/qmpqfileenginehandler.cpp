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
    QStringList suffixes = QMPQFileEngine::supportedFormats();
//    suffixes << "mpq" << "w3x" << "w3m";
    foreach (QString suffix, suffixes) {
        if (fileName.toLower().contains('.' + suffix))
//#warning under Linux may cause bug with case-sensitive files
            return new QMPQFileEngine(fileName);
    }
    return 0;
    //    return fileName.toLower().contains(".mpq") ? new QMPQFileEngine(fileName) : 0;
//    return QRegExp(".*(\\.mpq|\\.w3x).*").exactMatch(fileName.toLower()) ? new QMPQFileEngine(fileName.toLower()) : 0;
}
