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

    compressionEnum = mo.enumerator(index);;

    for (int index = 0; index < me.keyCount(); index++) {
//        qDebug() << me.key(index);
        ui->comboBox->addItem(me.key(index));
        ui->comboBox->setItemData(index, me.value(index), Qt::UserRole);
    }

//    ui->treeWidget->setItemDelegateForColumn(1, new ComboBoxDelegate(this));

//    connect(ui->treeWidget, SIGNAL(activated(QModelIndex)), SLOT(onActivate(QModelIndex)));
    connect(ui->treeWidget, SIGNAL(clicked(QModelIndex)), SLOT(onClick(QModelIndex)));
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

    QTreeWidgetItem * item = new QTreeWidgetItem;
    ui->treeWidget->addTopLevelItem(item);

    const QMetaObject &mo = m_extensionManager->staticMetaObject;
    int indexOfEnumerator = mo.indexOfEnumerator("CompressionTypes");
    QMetaEnum me = mo.enumerator(indexOfEnumerator);

    item->setData(0, Qt::DisplayRole, key);
    item->setData(2, Qt::DisplayRole, compressionEnum.valueToKeys(value));
    item->setData(2, Qt::UserRole, value);
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
    addExtension("", 0);
}

void MPQSettings::removeExtension()
{
//    QList<QTableWidgetItem *> itemList = ui->treeWidget->selectedItems();
//    Q_ASSERT(itemList.isEmpty());

    QItemSelectionModel * model = ui->treeWidget->selectionModel();
    QModelIndexList indexes = model->selectedIndexes();

//    Q_ASSERT(!indexes.isEmpty());
    if (indexes.isEmpty())
        return;

    QModelIndex index = indexes.first();
    QModelIndex suffixIndex = ui->treeWidget->model()->index(index.row(), 0, index.parent());
    m_extensionManager->removeExtension(suffixIndex.data(Qt::DisplayRole).toString());

    model->clearSelection();
    model->select(ui->treeWidget->model()->index(index.row() + 1, 0, index.parent()), QItemSelectionModel::Select);
    model->select(ui->treeWidget->model()->index(index.row() + 1, 1, index.parent()), QItemSelectionModel::Select);
//    ui->treeWidget->removeRow(indexes.first().row());
    ui->treeWidget->takeTopLevelItem(indexes.first().row());
}

void MPQSettings::onClick(const QModelIndex & index)
{
    QAbstractItemModel * model = ui->treeWidget->model();
    QModelIndex suffixIndex = model->index(index.row(), 0, index.parent());
    ui->lineEdit->setText(model->data(suffixIndex, Qt::DisplayRole).toString());
    QModelIndex compressionIndex = model->index(index.row(), 2, index.parent());
    int compressionTypes = compressionIndex.data(Qt::UserRole).toInt();

    const QMetaObject &mo = m_extensionManager->staticMetaObject;
    int indexOfEnumerator = mo.indexOfEnumerator("CompressionTypes");
    QMetaEnum me = mo.enumerator(indexOfEnumerator);

//    int indexOfType = -1;
//    for (int i = 0; i < me.keyCount(); i++) {
//        if (me.value(i) == compressionTypes) {
//            indexOfType = i;
//            break;
//        }
//    }
    int indexOfType = compresionValueToIndex(compressionTypes);
    qDebug() << indexOfType;
    ui->comboBox->setCurrentIndex(indexOfType);
}

void MPQSettings::onEditText(const QString & text)
{
    QAbstractItemModel * model = ui->treeWidget->model();
    QItemSelectionModel * selectionModel = ui->treeWidget->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedRows();
    QModelIndex index = selectedIndexes.first();

    QString previousText = index.data(Qt::DisplayRole).toString();
    MPQExtensionManager::CompressionTypes previousTypes = m_extensionManager->compressionTypes(previousText);
    m_extensionManager->removeExtension(previousText);
    m_extensionManager->addExtension(text, previousTypes);
    model->setData(index, text, Qt::DisplayRole);
}

void MPQSettings::onActivate(int index)
{
    QAbstractItemModel * model = ui->treeWidget->model();
    QItemSelectionModel * selectionModel = ui->treeWidget->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedRows();
    QModelIndex rowIndex = selectedIndexes.first();
    QModelIndex extensionIndex = model->index(rowIndex.row(), 0, rowIndex.parent());
    QModelIndex compressionIndex = model->index(rowIndex.row(), 2, rowIndex.parent());

    QString text = extensionIndex.data(Qt::DisplayRole).toString();
    int compressionTypes = ui->comboBox->itemData(index, Qt::UserRole).toInt();

    const QMetaObject &mo = m_extensionManager->staticMetaObject;
    int indexOfEnumerator = mo.indexOfEnumerator("CompressionTypes");
    QMetaEnum me = mo.enumerator(indexOfEnumerator);

    model->setData(compressionIndex, compressionEnum.valueToKeys(compressionTypes), Qt::DisplayRole);
    model->setData(compressionIndex, compressionTypes, Qt::UserRole);
    m_extensionManager->setCompressionTypes(text, (MPQExtensionManager::CompressionType)compressionTypes);
}

//QString MPQSettings::compresionValueToKeys(int value)
//{
//    return compressionEnum.valueToKeys(value);
//}
//
//int MPQSettings::compresionValue(int index)
//{
//    return compressionEnum.value(value);
//}

int MPQSettings::compresionValueToIndex(int value)
{
    for (int i = 0; i < compressionEnum.keyCount(); i++) {
        if (compressionEnum.value(i) == value) {
            return i;
        }
    }
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

void MPQSettingsPage::setDefaults()
{
    setValue("mpq", 0);
    setValue("mp3", 0);
    setValue("*", 0);
}

QVariant MPQSettingsPage::value(const QString & key)
{
    return QVariant(m_widget->extensionManager()->compressionTypes(key));
}

void MPQSettingsPage::setValue(const QString & key, const QVariant & value)
{
    m_widget->addExtension(key, value.toInt());
}
