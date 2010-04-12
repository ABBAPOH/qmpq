#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QList>

class TabContext;
class TabManager : public QObject
{
Q_OBJECT
public:
    explicit TabManager(QObject *parent = 0);
    TabContext * context() { return contexts.value(currentIndex); }
    TabContext * context(int index) { return contexts.value(index); }
    void addContext(int index, TabContext * context);
    void remove(int index) { contexts.removeAt(index); }

signals:

public slots:
    void changeCurrent(int index);

private:
    QList<TabContext*> contexts;
    int currentIndex;
};

#endif // TABMANAGER_H
