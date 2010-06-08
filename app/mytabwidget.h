#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QTabWidget>

class QTabBar;
class MyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MyTabWidget(QWidget *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void tabBarDoubleClicked();

public slots:

private:
    QTabBar * tabBar;
};

#endif // MYTABWIDGET_H
