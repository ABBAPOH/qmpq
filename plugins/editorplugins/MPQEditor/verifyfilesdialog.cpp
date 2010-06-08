#include "verifyfilesdialog.h"
#include "ui_verifyfilesdialog.h"

VerifyFilesDialog::VerifyFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerifyFilesDialog)
{
    ui->setupUi(this);
}

VerifyFilesDialog::~VerifyFilesDialog()
{
    delete ui;
}

void VerifyFilesDialog::addLine(const QString & file, const QString & message)
{
    QTreeWidgetItem * item = new QTreeWidgetItem;
    item->setText(0, file);
    item->setText(1, message);
    ui->treeWidget->addTopLevelItem(item);
}

void VerifyFilesDialog::changeEvent(QEvent *e)
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
