#include "setfilelocaledialog.h"
#include "ui_setfilelocaledialog.h"

#include <../../../../QMPQFileEngine/qmpqarchive.h>

SetFileLocaleDialog::SetFileLocaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetFileLocaleDialog)
{
    ui->setupUi(this);

    connect(ui->comboBox, SIGNAL(activated(int)), SLOT(onActivate(int)));
}

SetFileLocaleDialog::~SetFileLocaleDialog()
{
    delete ui;
}

void SetFileLocaleDialog::setArchive(QMPQArchive * archive)
{
    m_archive = archive;
    m_locales.append(QLocale(QLocale::C));
    m_locales.append(QLocale(QLocale::German, QLocale::Germany));
    m_locales.append(QLocale(QLocale::English, QLocale::UnitedKingdom));
    m_locales.append(QLocale(QLocale::English, QLocale::UnitedStates));
    m_locales.append(QLocale(QLocale::Spanish, QLocale::Spain));
    m_locales.append(QLocale(QLocale::French, QLocale::France));
    m_locales.append(QLocale(QLocale::Italian, QLocale::Italy));
    m_locales.append(QLocale(QLocale::Korean, QLocale::DemocraticRepublicOfKorea));
    m_locales.append(QLocale(QLocale::Polish, QLocale::Poland));
    m_locales.append(QLocale(QLocale::Russian, QLocale::RussianFederation));
    m_locales.append(QLocale(QLocale::Chinese, QLocale::China));

    foreach (QLocale locale, m_locales) {
        ui->comboBox->addItem(locale.name());
    }
}

void SetFileLocaleDialog::setFiles(QStringList files)
{
    m_files = files;
}

QLocale SetFileLocaleDialog::fileLocale()
{
    return m_locales.at(ui->comboBox->currentIndex());
}

void SetFileLocaleDialog::setFileLocale(QLocale locale)
{
    ui->comboBox->setCurrentIndex(m_locales.indexOf(locale));
}

void SetFileLocaleDialog::onActivate(int index)
{
}
