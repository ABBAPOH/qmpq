#ifndef COMPACTPROCESSDIALOG_H
#define COMPACTPROCESSDIALOG_H

#include <QDialog>
#include <qmpqarchive.h>

namespace Ui {
    class CompactProcessDialog;
}

class CompactProcessDialog : public QDialog {
    Q_OBJECT
public:
    CompactProcessDialog(QWidget *parent = 0);
    ~CompactProcessDialog();

protected:
    void changeEvent(QEvent *e);

private:
    QString getOperation(QMPQArchive::CompactOperation op);
    Ui::CompactProcessDialog *ui;
    int previousOperation;

public slots:
    void setProgress(QMPQArchive::CompactOperation op, qint64 bytesProcessed, qint64 bytesTotal);
};

#endif // COMPACTPROCESSDIALOG_H
