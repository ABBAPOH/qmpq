#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "iactionmanager.h"
#include <QtCore/QMap>
#include <QtCore/QString>

class ActionManager : public IActionManager
{
Q_OBJECT
public:
    explicit ActionManager(QObject *parent = 0);

    QAction * action(const QString & id);
    QMenu * createMenu(const QString & id);
    QMenu * menu(const QString & id);
    QList<QString> menuIds();
    void registerAction(QAction * action, const QString & id);

signals:

public slots:
    void undo();
    void redo();

    void cut();
    void copy();
    void paste();

private slots:
    void updateActions();
    void updateSave_AsAction();
    void updateSaveAction();

private:
    QMap<QString, QMenu *> m_menus;
    QMap<QString, QAction *> m_actions;
};

#endif // ACTIONMANAGER_H
