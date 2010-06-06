#include "qtabbarex.h"

#include <QDragEnterEvent>
#include <QTimer>
#include <QDebug>

QTabBarEx::QTabBarEx(QWidget *parent) :
    QTabBar(parent),
    m_timer(new QTimer(this))
{
    m_tabIndex = -1;
    m_switchTabsOnDrag = false;
    m_switchOnHover = false;
    m_switchTimeout = 500;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(changeTab()));
}

/*!
    \property QTabBarEx::switchTabsOnDrag
    \brief If set to true, switches current tab while user drags something over the tab

    By default, this property is false;
*/
bool QTabBarEx::switchTabsOnDrag()
{
    return m_switchTabsOnDrag;
}

void QTabBarEx::setSwitchTabsOnDrag(bool switchTabsOnDrag)
{
    if (m_switchTabsOnDrag == switchTabsOnDrag)
        return;
    setAcceptDrops(switchTabsOnDrag);
    m_switchTabsOnDrag = switchTabsOnDrag;
}

/*!
    \property QTabBarEx::switchOnHover
    \brief If set to true, tab is switched moving mouse over it, otherwise
    it is switched after mouse being stopped.

    By default, this property is false;
*/
bool QTabBarEx::switchOnHover()
{
    return m_switchOnHover;
}

void QTabBarEx::setSwitchOnHover(bool switchOnHover)
{
    m_switchOnHover = switchOnHover;
}

/*!
    \property QTabBarEx::switchTimeout
    \brief Timeout before tab is switched while performing drag'n'drop operation

    Default value is 500 milliseconds.
*/
int QTabBarEx::switchTimeout()
{
    return m_switchTimeout;
}

void QTabBarEx::setSwitchTimeout(int milliseconds)
{
    m_switchTimeout = milliseconds;
}

/*!\reimp
 */
void QTabBarEx::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "QTabBarEx::dragEnterEvent";
    if (m_switchTabsOnDrag)
        event->accept();
    else
        event->ignore();
}

/*!\reimp
 */
void QTabBarEx::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "QTabBarEx::dragMoveEvent";
    event->ignore();
    int tabIndex = tabAt(event->pos());
    if (!m_switchOnHover || m_tabIndex != tabIndex) {
        m_tabIndex = tabIndex;
        m_timer->stop();
        m_timer->start(m_switchTimeout);
    }
}

/*!\reimp
 */
void QTabBarEx::dragLeaveEvent(QDragLeaveEvent * event)
{
    m_timer->stop();
}

void QTabBarEx::changeTab()
{
//    qDebug() << "QTabBarEx::changeTab";
    setCurrentIndex(m_tabIndex);
    m_timer->stop();
}
