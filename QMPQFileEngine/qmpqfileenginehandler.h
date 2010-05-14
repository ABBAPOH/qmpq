#ifndef QMPQFILEENGINEHANDLER_H
#define QMPQFILEENGINEHANDLER_H

#include "QMPQFileEngine_global.h"

#include <QAbstractFileEngineHandler>
#include <QString>

class QMPQFILEENGINESHARED_EXPORT QMPQFileEngineHandler : public QAbstractFileEngineHandler
{
public:
    QMPQFileEngineHandler();
    QAbstractFileEngine *create(const QString &fileName) const;
};

#endif // QMPQFILEENGINEHANDLER_H
