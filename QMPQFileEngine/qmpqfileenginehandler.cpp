#include "qmpqfileenginehandler.h"

#include "qmpqfileengine.h"

#include <QtCore/QDebug>

bool QMPQFileEngineHandler::lock = false;

QMPQFileEngineHandler::QMPQFileEngineHandler()
{
//    qDebug() << "QMPQFileEngineHandler::QMPQFileEngineHandler";
}

//QAbstractFileEngine *QMPQFileEngineHandler::create(const QString &fileName, QAbstractFileEngine::EngineType type) const
QAbstractFileEngine *QMPQFileEngineHandler::create(const QString &fileName) const
{
    if (lock)
        return 0;
    QStringList suffixes = QMPQFileEngine::supportedFormats();
//    suffixes << "mpq" << "w3x" << "w3m";
    foreach (QString suffix, suffixes) {
        int index = fileName.lastIndexOf('.' + suffix);
        if (index != -1) {
//            qDebug() << fileName.length() << index << type << fileName;
//#warning under Linux may cause bug with case-sensitive files
//            if (type == QAbstractFileEngine::File && fileName.length() == index+4)
//                return 0;
//                qDebug() << "zero";
//            else
                return new QMPQFileEngine(fileName);
        }
    }
    return 0;
    //    return fileName.toLower().contains(".mpq") ? new QMPQFileEngine(fileName) : 0;
//    return QRegExp(".*(\\.mpq|\\.w3x).*").exactMatch(fileName.toLower()) ? new QMPQFileEngine(fileName.toLower()) : 0;
}
