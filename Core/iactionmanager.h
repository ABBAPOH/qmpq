#ifndef IACTIONMANAGER_H
#define IACTIONMANAGER_H

#include <QtCore/QObject>

class QAction;
class QMenu;
class QString;
class IActionManager : public QObject
{
    Q_OBJECT
public:
    IActionManager(QObject * parent = 0) : QObject(parent) {}

    virtual QAction * action(const QString & id) = 0;
    virtual QMenu * createMenu(const QString & id) = 0;
    virtual QMenu * menu(const QString & id) = 0;
    virtual QList<QString> menuIds() = 0;
    virtual void registerAction(QAction * action, const QString & id) = 0;
};

#endif // IACTIONMANAGER_H
