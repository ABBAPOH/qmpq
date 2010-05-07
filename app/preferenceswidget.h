#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include <QtCore/QHash>
#include <QtGui/QWidget>

namespace Ui {
    class PreferencesWidget;
}

class PreferencesManager;
class QTreeWidgetItem;
class PreferencesWidget : public QWidget {
    Q_OBJECT
public:
//    PreferencesWidget(QWidget *parent = 0);
    PreferencesWidget(PreferencesManager * manager, QWidget *parent = 0);
    ~PreferencesWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PreferencesWidget *ui;
    PreferencesManager * m_manager;
    QHash<QString, QTreeWidgetItem *> m_items;
private slots:
    void onItemClick(QTreeWidgetItem *, int);
};

#endif // PREFERENCESWIDGET_H
