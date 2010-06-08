#ifndef VERIFYFILESDIALOG_H
#define VERIFYFILESDIALOG_H

#include <QDialog>

namespace Ui {
    class VerifyFilesDialog;
}

class VerifyFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VerifyFilesDialog(QWidget *parent = 0);
    ~VerifyFilesDialog();
    void addLine(const QString & file, const QString & message);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::VerifyFilesDialog *ui;
};

#endif // VERIFYFILESDIALOG_H
