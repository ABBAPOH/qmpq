#include "tabmanager.h"

TabManager::TabManager(QObject *parent) :
    QObject(parent)
{
}

void TabManager::addContext(int index, TabContext * context)
{
//    if(index < contexts.size())
    contexts.insert(index, context);
}

void TabManager::changeCurrent(int index)
{
    currentIndex = index;
}
