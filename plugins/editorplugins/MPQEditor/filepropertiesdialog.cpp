#include "filepropertiesdialog.h"
#include "ui_filepropertiesdialog.h"

#include <QMetaEnum>

#include "../../../../QMPQFileEngine/qmpqfileengine.h"
#include "../../../../QMPQFileEngine/qmpqarchive.h"
#include "../../../../QMPQFileEngine/mpqfileinfo.h"

FilePropertiesDialog::FilePropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilePropertiesDialog)
{
    ui->setupUi(this);
}

FilePropertiesDialog::~FilePropertiesDialog()
{
    delete ui;
}

void FilePropertiesDialog::setInfo(QMPQArchive * archive, const QString & file)
{
    m_archive = archive;

    QLocale locale = QMPQFileEngine::localeFromName(file);
    archive->setLocale(locale);
    MPQFileInfo info = archive->fileInfo(QMPQFileEngine::fileNameFromName(file));
    archive->setLocale(QLocale(QLocale::C));

    QTreeWidgetItem * item = 0;

    item = ui->treeWidget->topLevelItem(0);
    item->setData(1, Qt::DisplayRole, archive->file());

    item = ui->treeWidget->topLevelItem(1);
    item->setData(1, Qt::DisplayRole, file);

    item = ui->treeWidget->topLevelItem(2);
    item->setData(1, Qt::DisplayRole, info.hashIndex());

    item = ui->treeWidget->topLevelItem(3);
    item->setData(1, Qt::DisplayRole, info.codename1());

    item = ui->treeWidget->topLevelItem(4);
    item->setData(1, Qt::DisplayRole, info.codename2());

    item = ui->treeWidget->topLevelItem(5);
    item->setData(1, Qt::DisplayRole, info.blockIndex());

    item = ui->treeWidget->topLevelItem(6);
    item->setData(1, Qt::DisplayRole, info.fileSize());

    item = ui->treeWidget->topLevelItem(7);
    item->setData(1, Qt::DisplayRole, info.compressedSize());

    item = ui->treeWidget->topLevelItem(8);
    item->setData(1, Qt::DisplayRole, QString().sprintf("%d%%", (unsigned)(100.0*info.fileSize()/info.compressedSize())));

    item = ui->treeWidget->topLevelItem(10);
    item->setData(1, Qt::DisplayRole, info.locale().name());

    item = ui->treeWidget->topLevelItem(11);
    QMetaObject mo = archive->staticMetaObject;
    QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("FileFlags"));
    item->setData(1, Qt::DisplayRole, me.valueToKeys(info.fileFlags()));
}

void FilePropertiesDialog::changeEvent(QEvent *e)
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
