#include "mytabwidget.h"

#include "qtabbarex.h"

MyTabWidget::MyTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    QTabBarEx * tabBar = new QTabBarEx(this);
    tabBar->setSwitchTabsOnDrag(true);
//    tabBar->setSwitchOnHover(true);
//    tabBar->setSwitchInterval(1000);
    setTabBar(tabBar);
}
