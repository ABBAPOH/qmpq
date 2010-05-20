#include "hashtablesizedialog.h"
#include "ui_hashtablesizedialog.h"

HashTableSizeDialog::HashTableSizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HashTableSizeDialog)
{
    ui->setupUi(this);
}

HashTableSizeDialog::~HashTableSizeDialog()
{
    delete ui;
}

int HashTableSizeDialog::hashTableSize()
{
    return ui->spinBox->value();
}

void HashTableSizeDialog::setHashTableSize(int size)
{
    ui->spinBox->setValue(size);
}

void HashTableSizeDialog::changeEvent(QEvent *e)
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
