#include "qmpqfileenginehandler.h"

#include "qmpqfileengine.h"
#include "qmpqarchivecache.h"

#include <QtCore/QDebug>

QMPQFileEngineHandler::QMPQFileEngineHandler() : QAbstractFileEngineHandler()
{
    QMPQArchiveCache::init();
}

QAbstractFileEngine *QMPQFileEngineHandler::create(const QString &fileName) const
{
    if (fileName.startsWith("mpq:")) {
        QMPQFileEngine * engine = new QMPQFileEngine;
        engine->setFileName(fileName);
        if (!engine->isCreated()) {
            delete engine;
            return 0;
        }

        return engine;
    }
    return 0;
}
