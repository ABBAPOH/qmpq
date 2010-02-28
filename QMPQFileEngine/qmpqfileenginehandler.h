#ifndef QMPQFILEENGINEHANDLER_H
#define QMPQFILEENGINEHANDLER_H

#include "QMPQFileEngine_global.h"

#include <QAbstractFileEngineHandler>
#include <QString>

class QMPQFILEENGINESHARED_EXPORT QMPQFileEngineHandler : public QAbstractFileEngineHandler
{
    static bool lock;
public:
    QMPQFileEngineHandler();
//    QAbstractFileEngine *create(const QString &fileName, QAbstractFileEngine::EngineType type) const;
    QAbstractFileEngine *create(const QString &fileName) const;
    static void setLocked(bool locked) { lock = locked; }
};

#endif // QMPQFILEENGINEHANDLER_H
