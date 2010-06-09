#ifndef ARCHIVEPROPERTIESDIALOG_H
#define ARCHIVEPROPERTIESDIALOG_H

#include <QDialog>

namespace Ui {
    class ArchivePropertiesDialog;
}

class QMPQArchive;
class ArchivePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArchivePropertiesDialog(QWidget *parent = 0);
    ~ArchivePropertiesDialog();

    void setArchive(QMPQArchive * archive);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ArchivePropertiesDialog *ui;
};

#endif // ARCHIVEPROPERTIESDIALOG_H
