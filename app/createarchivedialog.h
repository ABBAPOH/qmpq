#ifndef CREATEARCHIVEDIALOG_H
#define CREATEARCHIVEDIALOG_H

#include "ui_createarchivedialog.h"

class CreateArchiveDialog: public Ui::CreateArchiveDialog, public QDialog
{
    public:
    CreateArchiveDialog(QWidget *parent = 0) :
            QDialog(parent)
    {
        setupUi(this);
    }

    int maxFiles()
    {
        return spinBox->value();
    }

    int archiveVersion()
    {
        return v1Button->isChecked() ? 1 : 2;
    }

    bool addAttributes()
    {
        return attributesButton->isChecked();
    }
};

#endif // CREATEARCHIVEDIALOG_H
