#include "mytabwidget.h"

#include "qtabbarex.h"

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>

#include <QDebug>

MyTabWidget::MyTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    QTabBarEx * tabBar = new QTabBarEx(this);
    tabBar->setSwitchTabsOnDrag(true);
    tabBar->installEventFilter(this);
//    tabBar->setSwitchOnHover(true);
//    tabBar->setSwitchInterval(1000);
    this->tabBar = tabBar;
    setTabBar(tabBar);
}

bool MyTabWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(event);
        if (tabBar->tabAt(mouseEvent->pos()) == -1)
            emit tabBarDoubleClicked();
        return true;
    } else {
        // standard event processing
        return QTabWidget::eventFilter(obj, event);
    }
}
