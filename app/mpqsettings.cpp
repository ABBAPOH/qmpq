#include "mpqsettings.h"
#include "ui_mpqsettings.h"

#include "../QMPQFileEngine/mpqextensionmanager.h"

#include <QMetaEnum>

#include <QDebug>

MPQSettings::MPQSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MPQSettings)
{
    m_extensionManager = MPQExtensionManager::instance();
    MPQExtensionManager::CompressionTypes types;
    qDebug() << types;
    ui->setupUi(this);
    const QMetaObject &mo = m_extensionManager->staticMetaObject;
    int index = mo.indexOfEnumerator("CompressionTypes");
    QMetaEnum me = mo.enumerator(index);
    for (int index = 0; index < me.keyCount(); index++) {
//        qDebug() << me.key(index);
        ui->comboBox->addItem(me.key(index));
        ui->comboBox->setItemData(index, me.value(index), Qt::UserRole);
    }

//    ui->tableWidget->setItemDelegateForColumn(1, new ComboBoxDelegate(this));

//    connect(ui->tableWidget, SIGNAL(activated(QModelIndex)), SLOT(onActivate(QModelIndex)));
    connect(ui->tableWidget, SIGNAL(clicked(QModelIndex)), SLOT(onClick(QModelIndex)));
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(addExtension()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(removeExtension()));
    connect(ui->lineEdit, SIGNAL(textEdited(QString)), SLOT(onEditText(QString)));
    connect(ui->comboBox, SIGNAL(activated(int)), SLOT(onActivate(int)));
}

MPQSettings::~MPQSettings()
{
    delete ui;
}

void MPQSettings::addExtension(const QString & key, int value)
{
    extensionManager()->addExtension(key, (MPQExtensionManager::CompressionType)value);

    int rows = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rows);
    QTableWidgetItem * extensionItem = new QTableWidgetItem(key);
    ui->tableWidget->setItem(rows, 0, extensionItem);

    const QMetaObject &mo = m_extensionManager->staticMetaObject;
    int indexOfEnumerator = mo.indexOfEnumerator("CompressionTypes");
    QMetaEnum me = mo.enumerator(indexOfEnumerator);

    QTableWidgetItem * typesItem = new QTableWidgetItem(me.valueToKey(value));
    typesItem->setData(Qt::UserRole, value);
    ui->tableWidget->setItem(rows, 1, typesItem);
}

void MPQSettings::changeEvent(QEvent *e)
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

void MPQSettings::addExtension()
{
    int rows = ui->tableWidget->rowCount();
//    ui->tableWidget->setRowCount(rows + 1);
    addExtension("", 0);
}

void MPQSettings::removeExtension()
{
    QItemSelectionModel * model = ui->tableWidget->selectionModel();
    QModelIndexList indexes = model->selectedRows();

    Q_ASSERT(indexes.isEmpty());

    ui->tableWidget->removeRow(indexes.first().row());
}

void MPQSettings::onClick(const QModelIndex & index)
{
    QTableWidgetItem * item = ui->tableWidget->item(index.row(), index.column());
    QAbstractItemModel * model = ui->tableWidget->model();
    QModelIndex suffixIndex = model->index(index.row(), 0, index.parent());
    ui->lineEdit->setText(model->data(suffixIndex, Qt::DisplayRole).toString());
    QModelIndex compressionIndex = model->index(index.row(), 1, index.parent());
    ui->comboBox->setCurrentIndex(compressionIndex.data(Qt::UserRole).toInt());
}

void MPQSettings::onEditText(const QString & text)
{
    QList<QTableWidgetItem *> itemList = ui->tableWidget->selectedItems();
    qDebug() << itemList.first()->row();

    QString previousText = itemList.first()->data(Qt::DisplayRole).toString();
    MPQExtensionManager::CompressionTypes previousTypes = m_extensionManager->compressionTypes(previousText);
    m_extensionManager->removeExtension(previousText);
    m_extensionManager->addExtension(text, previousTypes);
    itemList.first()->setData(Qt::DisplayRole, text);

//    QItemSelectionModel * model = ui->tableWidget->selectionModel();
//    QModelIndexList indexes = model->selectedIndexes();

//    Q_ASSERT(indexes.isEmpty());
//    QModelIndex index = ui->tableWidget->model()->index(indexes.first().row(), 0);
//    QTableWidgetItem * item = ui->tableWidget->item(index.row(), index.column());
//    item->setData(Qt::DisplayRole, text);
}

void MPQSettings::onActivate(int index)
{
    QList<QTableWidgetItem *> itemList = ui->tableWidget->selectedItems();
    qDebug() << ui->comboBox->itemData(index, Qt::UserRole);

    QTableWidgetItem * extensionItem = itemList.first();
    QString text = extensionItem->data(Qt::DisplayRole).toString();
    int compressionTypes = ui->comboBox->itemData(index, Qt::UserRole).toInt();
    QTableWidgetItem * typesItem = ui->tableWidget->item(extensionItem->row(), 1);

    const QMetaObject &mo = m_extensionManager->staticMetaObject;
    int indexOfEnumerator = mo.indexOfEnumerator("CompressionTypes");
    QMetaEnum me = mo.enumerator(indexOfEnumerator);

    typesItem->setData(Qt::DisplayRole, me.valueToKey(compressionTypes));
    typesItem->setData(Qt::UserRole, compressionTypes);
    m_extensionManager->setCompressionTypes(text, (MPQExtensionManager::CompressionType)compressionTypes);
}



#include <QtGui>
#include <QComboBox>
// #include "delegate.h"

 ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
     : QItemDelegate(parent)
 {
 }

 QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     qDebug() << "ComboBoxDelegate::createEditor";
     QComboBox *editor = new QComboBox(parent);
     editor->addItem("test1");
     editor->addItem("test2");
     editor->addItem("test3");
//     editor->setMinimum(0);
//     editor->setMaximum(100);

     return editor;
 }

 void ComboBoxDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     qDebug() << "ComboBoxDelegate::setEditorData";
     int value = index.model()->data(index, Qt::UserRole).toInt();

     QComboBox *comboBox = static_cast<QComboBox*>(editor);
     comboBox->setCurrentIndex(value);
     comboBox->showPopup();

//     spinBox->setValue(value);
 }

 Q_DECLARE_METATYPE(QComboBox*)

 void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QComboBox *comboBox = static_cast<QComboBox*>(editor);
//     comboBox->interpretText();
//     int value = comboBox->value();

//     QVariant data = QVariant::fromValue(comboBox);

//     model->setData(index, QPixmap::grabWidget(comboBox)/*data*//*value*/, Qt::DecorationRole);
     int currentIndex = comboBox->currentIndex();
     model->setData(index, comboBox->itemText(currentIndex));
     model->setData(index, currentIndex, Qt::UserRole);
 }

 void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }

// void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
//                 const QModelIndex &index) const
// {
//     qDebug() << "ComboBoxDelegate::paint";
////    QComboBox *comboBox = qVariantValue<QComboBox *>(index.data());
////     qApp->style()->drawControl(QStyle::CE_ComboBoxLabel, option, painter);
////    comboBox->show();
// }


QStringList MPQSettingsPage::keys()
{
    return m_widget->extensionManager()->extensions();
}

QVariant MPQSettingsPage::value(const QString & key)
{
    return QVariant(m_widget->extensionManager()->compressionTypes(key));
}

void MPQSettingsPage::setValue(const QString & key, const QVariant & value)
{
    m_widget->addExtension(key, value.toInt());
}
