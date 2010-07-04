#ifndef SETFILELOCALEDIALOG_H
#define SETFILELOCALEDIALOG_H

#include <QDialog>

namespace Ui {
    class SetFileLocaleDialog;
}

class QMPQArchive;
class SetFileLocaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetFileLocaleDialog(QWidget *parent = 0);
    ~SetFileLocaleDialog();
    void setArchive(QMPQArchive * archive);
    void setFiles(QStringList files);
    QLocale fileLocale();
    void setFileLocale(QLocale locale);

private slots:
    void onActivate(int index);

private:
    Ui::SetFileLocaleDialog *ui;
    QMPQArchive * m_archive;
    QStringList m_files;
    QList<QLocale> m_locales;
};

#endif // SETFILELOCALEDIALOG_H
