#include "archivepropertiesdialog.h"
#include "ui_archivepropertiesdialog.h"

#include <QMetaEnum>

#include "../../../../QMPQFileEngine/qmpqarchive.h"

ArchivePropertiesDialog::ArchivePropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArchivePropertiesDialog)
{
    ui->setupUi(this);
}

ArchivePropertiesDialog::~ArchivePropertiesDialog()
{
    delete ui;
}

void ArchivePropertiesDialog::setArchive(QMPQArchive * archive)
{
    QTreeWidgetItem * item = 0;

    item = ui->treeWidget->topLevelItem(0);
    item->setText(1, archive->file());

    item = ui->treeWidget->topLevelItem(2);
    item->setData(1, Qt::DisplayRole, archive->hashTableSize());

    item = ui->treeWidget->topLevelItem(4);
    item->setData(1, Qt::DisplayRole, archive->blockTableSize());

    item = ui->treeWidget->topLevelItem(5);
    item->setData(1, Qt::DisplayRole, archive->filesCount());

    item = ui->treeWidget->topLevelItem(6);
    item->setData(1, Qt::DisplayRole, archive->sectorSize());

    item = ui->treeWidget->topLevelItem(7);
    QMetaObject mo = archive->staticMetaObject;
    QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Attributes"));
    item->setData(1, Qt::DisplayRole, me.valueToKeys(archive->attributes()));
}

void ArchivePropertiesDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
