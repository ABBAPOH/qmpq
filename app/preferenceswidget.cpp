#include "preferenceswidget.h"
#include "ui_preferenceswidget.h"

#include <preferencesmanager.h>

//#include <QDebug>

PreferencesWidget::PreferencesWidget(PreferencesManager * manager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreferencesWidget),
    m_manager(manager)
{
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>() << 140 << 400);
//    ui->stackedWidget->addWidget(new QWidget);

    foreach (IPreferencesPage * page, manager->pages()) {
//        IPreferencesPage * page = manager->page(pageKey);
        QString groupKey = page->groupKey();
        addGroup(groupKey);

        QTabWidget * tabWidget = m_tabWidgets.value(groupKey);
        tabWidget->addTab(page->widget(), page->name());
    }
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(onItemClick(QTreeWidgetItem*,int)));
}

PreferencesWidget::~PreferencesWidget()
{
    delete ui;
}

void PreferencesWidget::addGroup(const QString & name)
{
    if (m_items.contains(name))
        return;
    QTreeWidgetItem * item = new QTreeWidgetItem();
    QTabWidget * tabWidget = new QTabWidget();
    m_tabWidgets.insert(name, tabWidget);
    int index = ui->stackedWidget->addWidget(tabWidget);
    m_items.insert(name, item);
    item->setText(0, name);
    item->setData(0, Qt::UserRole, index);
    m_indexes.insert(name, index);
    ui->treeWidget->addTopLevelItem(item);
}

void PreferencesWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PreferencesWidget::onItemClick(QTreeWidgetItem * item, int /*column*/)
{
    int index = item->data(0, Qt::UserRole).toInt();
    ui->stackedWidget->setCurrentIndex(index);
}
