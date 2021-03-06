#include "mpqsettings.h"
#include "ui_mpqsettings.h"

#include "../QMPQFileEngine/mpqsettings.h"

#include <QMetaEnum>

#include <QDebug>

MPQSettingsWidget::MPQSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MPQSettings)
{
    m_extensionManager = MPQSettings::instance();
    QMPQArchive::CompressionFlags types;
    qDebug() << types;
    ui->setupUi(this);
    const QMetaObject &mo = QMPQArchive::staticMetaObject;
    int index = mo.indexOfEnumerator("CompressionFlags");
    QMetaEnum me = mo.enumerator(index);

    optionsEnum = mo.enumerator(mo.indexOfEnumerator("FileFlags"));
    compressionEnum = mo.enumerator(index);

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

    connect(ui->checkBox_Implode, SIGNAL(toggled(bool)), SLOT(onCheck()));
    connect(ui->checkBox_Compress, SIGNAL(toggled(bool)), SLOT(onCheck()));
    connect(ui->checkBox_Encrypt, SIGNAL(toggled(bool)), SLOT(onCheck()));
    connect(ui->checkBox_File_Key, SIGNAL(toggled(bool)), SLOT(onCheck()));
    connect(ui->checkBox_SingleUnit, SIGNAL(toggled(bool)), SLOT(onCheck()));
    connect(ui->checkBox_Deleted, SIGNAL(toggled(bool)), SLOT(onCheck()));
    connect(ui->checkBox_CheckSum, SIGNAL(toggled(bool)), SLOT(onCheck()));
}

MPQSettingsWidget::~MPQSettingsWidget()
{
    delete ui;
}

void MPQSettingsWidget::addExtension(const QString & key, int options, int value)
{
    extensionManager()->addFileExtension(key, (QMPQArchive::FileFlag)options, (QMPQArchive::CompressionFlag)value);

    QTreeWidgetItem * item = new QTreeWidgetItem;
    ui->treeWidget->addTopLevelItem(item);

    item->setData(0, Qt::DisplayRole, key);
    item->setData(1, Qt::DisplayRole, optionsEnum.valueToKeys(options));
    item->setData(1, Qt::UserRole, options);
    item->setData(2, Qt::DisplayRole, compressionEnum.valueToKeys(value));
    item->setData(2, Qt::UserRole, value);
}

void MPQSettingsWidget::changeEvent(QEvent *e)
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

void MPQSettingsWidget::addExtension()
{
    addExtension("", 0, 0);
}

void MPQSettingsWidget::removeExtension()
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
    m_extensionManager->removeFileExtension(suffixIndex.data(Qt::DisplayRole).toString());

    model->clearSelection();
    model->select(ui->treeWidget->model()->index(index.row() + 1, 0, index.parent()), QItemSelectionModel::Select);
    model->select(ui->treeWidget->model()->index(index.row() + 1, 1, index.parent()), QItemSelectionModel::Select);
//    ui->treeWidget->removeRow(indexes.first().row());
    ui->treeWidget->takeTopLevelItem(indexes.first().row());
}

void MPQSettingsWidget::onClick(const QModelIndex & index)
{
    QAbstractItemModel * model = ui->treeWidget->model();
    QModelIndex suffixIndex = model->index(index.row(), 0, index.parent());
    QModelIndex optionsIndex = model->index(index.row(), 1, index.parent());
    QModelIndex compressionIndex = model->index(index.row(), 2, index.parent());

    ui->lineEdit->setText(model->data(suffixIndex, Qt::DisplayRole).toString());
    int compressionTypes = compressionIndex.data(Qt::UserRole).toInt();
    int options = optionsIndex.data(Qt::UserRole).toInt();

    setCurrentOptions(options);

    int indexOfType = compresionValueToIndex(compressionTypes);
    qDebug() << indexOfType;
    ui->comboBox->setCurrentIndex(indexOfType);
}

void MPQSettingsWidget::onEditText(const QString & text)
{
    QAbstractItemModel * model = ui->treeWidget->model();
    QItemSelectionModel * selectionModel = ui->treeWidget->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedRows();
    QModelIndex index = selectedIndexes.first();

    QString previousText = index.data(Qt::DisplayRole).toString();
    QMPQArchive::CompressionFlags previousTypes = m_extensionManager->compressionFlags(previousText);
    m_extensionManager->removeFileExtension(previousText);
    m_extensionManager->addFileExtension(text, 0, previousTypes);
    model->setData(index, text, Qt::DisplayRole);
}

void MPQSettingsWidget::onActivate(int index)
{
    QAbstractItemModel * model = ui->treeWidget->model();
    QItemSelectionModel * selectionModel = ui->treeWidget->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedRows();
    QModelIndex rowIndex = selectedIndexes.first();
    QModelIndex extensionIndex = model->index(rowIndex.row(), 0, rowIndex.parent());
    QModelIndex compressionIndex = model->index(rowIndex.row(), 2, rowIndex.parent());

    QString text = extensionIndex.data(Qt::DisplayRole).toString();
    int compressionTypes = ui->comboBox->itemData(index, Qt::UserRole).toInt();

    const QMetaObject &mo = QMPQArchive::staticMetaObject;
    int indexOfEnumerator = mo.indexOfEnumerator("CompressionFlags");
    QMetaEnum me = mo.enumerator(indexOfEnumerator);

    model->setData(compressionIndex, compressionEnum.valueToKeys(compressionTypes), Qt::DisplayRole);
    model->setData(compressionIndex, compressionTypes, Qt::UserRole);
    m_extensionManager->setCompressionFlags(text, (QMPQArchive::CompressionFlag)compressionTypes);
}

void MPQSettingsWidget::onCheck()
{
    QAbstractItemModel * model = ui->treeWidget->model();
    QItemSelectionModel * selectionModel = ui->treeWidget->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedRows();
    QModelIndex rowIndex = selectedIndexes.first();
    QModelIndex extensionIndex = model->index(rowIndex.row(), 0, rowIndex.parent());
    QModelIndex optionsIndex = model->index(rowIndex.row(), 1, rowIndex.parent());

    QString extension = extensionIndex.data(Qt::DisplayRole).toString();
    int options = getCurrentOptions();

    model->setData(optionsIndex, optionsEnum.valueToKeys(options), Qt::DisplayRole);
    model->setData(optionsIndex, options, Qt::UserRole);
    m_extensionManager->setFileFlags(extension, QMPQArchive::FileFlags(options));
}


//QString MPQSettingsWidget::compresionValueToKeys(int value)
//{
//    return compressionEnum.valueToKeys(value);
//}
//
//int MPQSettingsWidget::compresionValue(int index)
//{
//    return compressionEnum.value(value);
//}

int MPQSettingsWidget::compresionValueToIndex(int value)
{
    for (int i = 0; i < compressionEnum.keyCount(); i++) {
        if (compressionEnum.value(i) == value) {
            return i;
        }
    }
    return -1;
}

int MPQSettingsWidget::getCurrentOptions()
{
    QMPQArchive::FileFlags flags = 0;

    if (ui->checkBox_Implode->isChecked())
        flags |= QMPQArchive::Implode;
    if (ui->checkBox_Compress->isChecked())
        flags |= QMPQArchive::Compress;
    if (ui->checkBox_Encrypt->isChecked())
        flags |= QMPQArchive::Encrypted;
    if (ui->checkBox_File_Key->isChecked())
        flags |= QMPQArchive::FixKey;
    if (ui->checkBox_SingleUnit->isChecked())
        flags |= QMPQArchive::SingleUnit;
    if (ui->checkBox_Deleted->isChecked())
        flags |= QMPQArchive::DeleteMarker;
    if (ui->checkBox_CheckSum->isChecked())
        flags |= QMPQArchive::SectorCRC;
    return flags;
}

void MPQSettingsWidget::setCurrentOptions(int value)
{
    QMPQArchive::FileFlags flags(value);

    ui->checkBox_Implode->setChecked(flags.testFlag(QMPQArchive::Implode));
    ui->checkBox_Compress->setChecked(flags.testFlag(QMPQArchive::Compress));
    ui->checkBox_Encrypt->setChecked(flags.testFlag(QMPQArchive::Encrypted));
    ui->checkBox_File_Key->setChecked(flags.testFlag(QMPQArchive::FixKey));
    ui->checkBox_SingleUnit->setChecked(flags.testFlag(QMPQArchive::SingleUnit));
    ui->checkBox_Deleted->setChecked(flags.testFlag(QMPQArchive::DeleteMarker));
    ui->checkBox_CheckSum->setChecked(flags.testFlag(QMPQArchive::SectorCRC));
}

//#include <QtGui>
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
    return m_widget->extensionManager()->fileExtensions();
}

void MPQSettingsPage::setDefaults()
{
    setValue("mpq", 0);
    setValue("mp3", 0);
    setValue("*", 0);
}

QVariant MPQSettingsPage::value(const QString & key)
{
    int options = m_widget->extensionManager()->fileFlags(key);
    int compresssion = m_widget->extensionManager()->compressionFlags(key);

    QMetaObject mo = QMPQArchive::staticMetaObject;
    QMetaEnum me1 = mo.enumerator(mo.indexOfEnumerator("FileFlags"));
    QMetaEnum me2 = mo.enumerator(mo.indexOfEnumerator("CompressionFlags"));

    QStringList result;
    result << me1.valueToKeys(options);
    result << me2.valueToKeys(compresssion);

//    qDebug() << "MPQSettingsPage::value" << key << result << options << compresssion;
    return QVariant(result);
}

void MPQSettingsPage::setValue(const QString & key, const QVariant & value)
{
    QStringList data = value.toStringList();

    QMetaObject mo = QMPQArchive::staticMetaObject;
    QMetaEnum me1 = mo.enumerator(mo.indexOfEnumerator("FileFlags"));
    QMetaEnum me2 = mo.enumerator(mo.indexOfEnumerator("CompressionFlags"));

    int options = 0;
    int compresssion = 0;
    if (data.count() == 2) {
        options = me1.keysToValue(data.at(0).toLocal8Bit().data());
        compresssion = me2.keysToValue(data.at(1).toLocal8Bit().data());
    }
//    qDebug() << "MPQSettingsPage::setValue" << data << options << compresssion;

    m_widget->addExtension(key, options, compresssion);
}
