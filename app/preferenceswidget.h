#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include <QtCore/QHash>
#include <QtGui/QWidget>

namespace Ui {
    class PreferencesWidget;
}

class PreferencesManager;
class QTreeWidgetItem;
class QTabWidget;
class PreferencesWidget : public QWidget {
    Q_OBJECT
public:
//    PreferencesWidget(QWidget *parent = 0);
    PreferencesWidget(PreferencesManager * manager, QWidget *parent = 0);
    ~PreferencesWidget();
    void addGroup(const QString & name);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PreferencesWidget *ui;
    PreferencesManager * m_manager;
    QHash<QString, QTreeWidgetItem *> m_items;
    QHash<QString, int> m_indexes;
    QHash<QString, QTabWidget *> m_tabWidgets;
//    QHash<QString, QTreeWidgetItem *> m_items;
private slots:
    void onItemClick(QTreeWidgetItem *, int);
};

#endif // PREFERENCESWIDGET_H
