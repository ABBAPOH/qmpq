#ifndef HASHTABLESIZEDIALOG_H
#define HASHTABLESIZEDIALOG_H

#include <QDialog>

namespace Ui {
    class HashTableSizeDialog;
}

class HashTableSizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HashTableSizeDialog(QWidget *parent = 0);
    ~HashTableSizeDialog();

    int hashTableSize();
    void setHashTableSize(int size);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::HashTableSizeDialog *ui;
};

#endif // HASHTABLESIZEDIALOG_H
