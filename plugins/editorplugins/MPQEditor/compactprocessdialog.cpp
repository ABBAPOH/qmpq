#include "compactprocessdialog.h"
#include "ui_compactprocessdialog.h"

CompactProcessDialog::CompactProcessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompactProcessDialog),
    previousOperation(0)
{
    ui->setupUi(this);
}

CompactProcessDialog::~CompactProcessDialog()
{
    delete ui;
}

void CompactProcessDialog::changeEvent(QEvent *e)
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

QString CompactProcessDialog::getOperation(QMPQArchive::CompactOperation op)
{
    switch(op) {
    case QMPQArchive::CheckingFiles : return tr("Checking files");
    case QMPQArchive::CheckingHashTableSize : return tr("Checking Hash Table Size");
    case QMPQArchive::CopyingNonMPQData : return tr("Copying non-MPQ Data");
    case QMPQArchive::CompactingFiles : return tr("Compacting files");
    case QMPQArchive::ClosingArchive : return tr("Closing Archive");
    default: return "";
    }
}

void CompactProcessDialog::setProgress(QMPQArchive::CompactOperation op, qint64 bytesProcessed, qint64 bytesTotal)
{
    if (previousOperation != op) {
        previousOperation = op;
        ui->label_Operation->setText(getOperation(op));
        ui->progressBar->setMaximum(bytesTotal);
    }
    ui->label_Bytes->setText(QString().sprintf("%d / %d", (int)bytesProcessed, (int)bytesTotal));
    ui->progressBar->setValue(bytesProcessed);
    QApplication::processEvents();
}

