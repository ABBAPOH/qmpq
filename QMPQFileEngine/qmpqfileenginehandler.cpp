#include "qmpqfileenginehandler.h"

#include "qmpqfileengine.h"
#include "qmpqarchivecache.h"

#include <QtCore/QDebug>

QMPQFileEngineHandler::QMPQFileEngineHandler() : QAbstractFileEngineHandler()
{
//    qDebug() << "QMPQFileEngineHandler::QMPQFileEngineHandler";
    QMPQArchiveCache::init();
}

QAbstractFileEngine *QMPQFileEngineHandler::create(const QString &fileName) const
{
    QStringList suffixes = QMPQFileEngine::supportedFormats();

//    if (QMPQFileEngineStringParser(fileName, suffixes).isFound()) {
    if (fileName.startsWith("mpq:")) {
        QMPQFileEngine * engine = new QMPQFileEngine;
        engine->setFileName(fileName);
        if (!engine->isCreated()) {
            delete engine;
            return 0;
        }

        return engine;
    }
//    const QStringList & names = fileName.split('/');
//    //    foreach (const QString & name, names) {
//    for (int i = names.count() - 1; i >= 0; i--) { // iterates list in a reverse order to support nested archives
//        const QString & name = names.at(i);
//        foreach (const QString & suffix, suffixes) {
//            if (name.endsWith('.' + suffix, Qt::CaseInsensitive)) {
//                qDebug() << suffix << fileName;
//                return new QMPQFileEngine(fileName);
//            }
//        }
//    }
    return 0;
}
