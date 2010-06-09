#include "changeattributesdialog.h"
#include "ui_changeattributesdialog.h"

ChangeAttributesDialog::ChangeAttributesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeAttributesDialog)
{
    ui->setupUi(this);
}

ChangeAttributesDialog::~ChangeAttributesDialog()
{
    delete ui;
}

QMPQArchive::Attributes ChangeAttributesDialog::attributes()
{
    QMPQArchive::Attributes attributes;
    if (ui->checkBox_inludeTime->isChecked())
        attributes |= QMPQArchive::A_FileTime;
    if (ui->checkBox_includeCRC32->isChecked())
        attributes |= QMPQArchive::A_CRC32;
    if (ui->checkBox_includeMD5->isChecked())
        attributes |= QMPQArchive::A_MD5;
    return attributes;
}

void ChangeAttributesDialog::setAttributes(QMPQArchive::Attributes attributes)
{
    ui->checkBox_inludeTime->setChecked(attributes.testFlag(QMPQArchive::A_FileTime));
    ui->checkBox_includeCRC32->setChecked(attributes.testFlag(QMPQArchive::A_CRC32));
    ui->checkBox_includeMD5->setChecked(attributes.testFlag(QMPQArchive::A_MD5));
}

bool ChangeAttributesDialog::updateFiles()
{
    return ui->checkBox_updateChecksums->isChecked();
}

void ChangeAttributesDialog::changeEvent(QEvent *e)
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
