#include "preferenceswidget.h"
#include "ui_preferenceswidget.h"

#include <preferencesmanager.h>

#include <QDebug>

PreferencesWidget::PreferencesWidget(PreferencesManager * manager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreferencesWidget),
    m_manager(manager)
{
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>() << 140 << 400);
    ui->stackedWidget->addWidget(new QWidget);

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(onItemClick(QTreeWidgetItem*,int)));

    QStringList keys = manager->pagesKeys();
    //  we sort list to have root items ve created BEFORE their childs
    keys.sort();
    foreach (QString pageKey, keys) {
        IPreferencesPage * page = manager->page(pageKey);

//        QString currentPageKey = "";
        int index = 0;
        int length = pageKey.length();
        QList<int> indexes;
        indexes.append(-1);
        for (int i = 0; i < length; i++) {
            if (pageKey.at(i) == '/')
                indexes.append(i);
        }
        indexes.append(length);
qDebug() << pageKey << indexes;
        QString parentPageKey = "";
        for (int i = 1; i < indexes.count(); i++) {
            QString currentPageKey = pageKey.mid(0, indexes.at(i));
            QString shortPageKey = pageKey.mid(indexes.at(i - 1) + 1, indexes.at(i));

            qDebug() << currentPageKey << currentPageKey;
            QTreeWidgetItem * parent = 0;
            if (parentPageKey != "")
                parent = m_items.value(parentPageKey);
            QTreeWidgetItem * item = m_items.value(currentPageKey);
            if (!item) {
                item = new QTreeWidgetItem(parent);
                m_items.insert(currentPageKey, item);
                item->setText(0, shortPageKey);
                item->setData(0, Qt::UserRole, 0);
           }
            parentPageKey = currentPageKey;
        }
        //  creates tree of items (does not fill item with pages)
        QTreeWidgetItem * item = m_items.value(pageKey);
        Q_ASSERT(item);
        int widgetIndex = ui->stackedWidget->addWidget(page->widget());
        item->setData(0, Qt::UserRole, widgetIndex);
        item->setText(0, page->name());
//        ui->treeWidget->addTopLevelItem(item);
    }
	foreach(QTreeWidgetItem * item, m_items.values()) {
		if (!item->parent())
            ui->treeWidget->addTopLevelItem(item);
	}
    ui->treeWidget->setColumnWidth(0, 300);
}

PreferencesWidget::~PreferencesWidget()
{
    delete ui;
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

void PreferencesWidget::onItemClick(QTreeWidgetItem * item, int column)
{
    qDebug () << item->text(0);
    int index = item->data(0, Qt::UserRole).toInt();
    ui->stackedWidget->setCurrentIndex(index);
}
