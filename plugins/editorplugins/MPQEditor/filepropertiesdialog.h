#ifndef FILEPROPERTIESDIALOG_H
#define FILEPROPERTIESDIALOG_H

#include <QDialog>

namespace Ui {
    class FilePropertiesDialog;
}

class QMPQArchive;
class FilePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilePropertiesDialog(QWidget *parent = 0);
    ~FilePropertiesDialog();

    void setInfo(QMPQArchive * archive, const QString & file);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FilePropertiesDialog *ui;
    QMPQArchive * m_archive;
};

#endif // FILEPROPERTIESDIALOG_H
