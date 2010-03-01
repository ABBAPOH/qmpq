#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTabWidget>

class TabWidget : public QTabWidget
{
Q_OBJECT
    QWidget * m_currentWidget; // just a container to correctly switch previousWidget
    QWidget * m_previousWidget;

public:
    explicit TabWidget(QWidget *parent = 0);
    QWidget * previousWidget();

signals:
    void	currentChanged(QWidget * widget); // inner signal

public slots:
    void newTab();
    void closeTab(int index);

private slots:
    void tabChanged(int index);
};

#endif // TREEWIDGET_H
