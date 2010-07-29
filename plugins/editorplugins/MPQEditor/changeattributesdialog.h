#ifndef CHANGEATTRIBUTESDIALOG_H
#define CHANGEATTRIBUTESDIALOG_H

#include <QDialog>
#include <qmpqarchive.h>

namespace Ui {
    class ChangeAttributesDialog;
}

class ChangeAttributesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeAttributesDialog(QWidget *parent = 0);
    ~ChangeAttributesDialog();

    QMPQArchive::Attributes attributes();
    void setAttributes(QMPQArchive::Attributes attributes);
    bool updateFiles();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ChangeAttributesDialog *ui;
};

#endif // CHANGEATTRIBUTESDIALOG_H
