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

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(onItemClick(QTreeWidgetItem*,int)));

    QStringList keys = manager->pagesKeys();
    //  we sort list to have root items ve created BEFORE their childs
    keys.sort();
    foreach(QString pageKey, keys) {
        IPreferencesPage * page = manager->page(pageKey);
        int index = pageKey.indexOf('/');
        QString name;
        if (index == -1)
            name = pageKey;
        else
            name = pageKey.left(index);
        //  creates tree of items (does not fill item with pages)
        do {
            QString parentPageKey = pageKey.left(index);
            qDebug() << parentPageKey;
            QTreeWidgetItem * parent = m_items.value(parentPageKey);
            QTreeWidgetItem * item = new QTreeWidgetItem(parent);
            m_items.insert(pageKey.left(index), item);
            int nextIndex = pageKey.indexOf('/', index + 1);
            if (nextIndex != -1)
                item->setText(0, pageKey.mid(index + 1, nextIndex));
            else
                item->setText(0, pageKey.mid(index + 1));
            index = nextIndex;
        } while (index != -1);
        QTreeWidgetItem * item = m_items.value(name);
        Q_ASSERT(item);
        int widgetIndex = ui->stackedWidget->addWidget(page->widget());
        item->setData(0, Qt::UserRole, widgetIndex);
        ui->treeWidget->addTopLevelItem(item);
    }
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
