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
        attributes |= QMPQArchive::FileTime;
    if (ui->checkBox_includeCRC32->isChecked())
        attributes |= QMPQArchive::CRC32;
    if (ui->checkBox_includeMD5->isChecked())
        attributes |= QMPQArchive::MD5;
    return attributes;
}

void ChangeAttributesDialog::setAttributes(QMPQArchive::Attributes attributes)
{
    ui->checkBox_inludeTime->setChecked(attributes.testFlag(QMPQArchive::FileTime));
    ui->checkBox_includeCRC32->setChecked(attributes.testFlag(QMPQArchive::CRC32));
    ui->checkBox_includeMD5->setChecked(attributes.testFlag(QMPQArchive::MD5));
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
