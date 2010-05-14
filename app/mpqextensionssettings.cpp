#include "mpqextensionssettings.h"
#include "ui_mpqextensionssettings.h"

#include "../QMPQFileEngine/mpqsettings.h"
#include "archivesuffixesmanager.h"

#include <QStringListModel>
#include <icore.h>

ArchiveSuffixesManager * manager()
{
    ArchiveSuffixesManager * manager =
            qobject_cast<ArchiveSuffixesManager*>(ICore::instance()->getObject("SuffixesManager"));
    Q_ASSERT(manager);
    return manager;
}

MPQExtensionsSettings::MPQExtensionsSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MPQExtensionsSettings),
    model(new QStringListModel(this)),
    settings(MPQSettings::instance())
{
    ui->setupUi(this);

    ui->listView->setModel(model);
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(addRow()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(removeRow()));
    connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(onDataChange(QModelIndex, QModelIndex)));
}

MPQExtensionsSettings::~MPQExtensionsSettings()
{
    delete ui;
}

void MPQExtensionsSettings::init()
{
    extensions = MPQSettings::instance()->MPQExtensions();
    model->setStringList(extensions);
}

void MPQExtensionsSettings::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MPQExtensionsSettings::addRow()
{
    int rows = model->rowCount(QModelIndex());
    model->insertRows(rows, 1, QModelIndex());
    ui->listView->edit(model->index(rows, 0, QModelIndex()));
    extensions.append("");
}

void MPQExtensionsSettings::removeRow()
{
    QItemSelectionModel * selectionModel = ui->listView->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    if (indexes.isEmpty())
        return;

    QModelIndex index = indexes.first();
    QString extension = index.data(Qt::DisplayRole).toString();
    extensions.removeAt(index.row());
    settings->removeMPQExtension(extension);
    manager()->removeSuffix(extension);
    model->removeRows(index.row(), 1, QModelIndex());
}

void MPQExtensionsSettings::onDataChange(QModelIndex topLeft, QModelIndex /*bottomRight*/)
{
    int row = topLeft.row();
    QString & extension = extensions[row];
    settings->removeMPQExtension(extension);
    manager()->removeSuffix(extension);
    extension = topLeft.data(Qt::DisplayRole).toString();
    settings->addMPQExtension(extension);
    manager()->addSuffix(extension, "mpq:");
}

QStringList MPQExtensionsPage::keys()
{
    return QStringList() << "extensions";
}

void MPQExtensionsPage::setDefaults()
{
    QStringList extensions;
    extensions <<  "mpq" << "w3x" << "w3m" << "s2ma" << "SC2Data" << "SC2Archive" << "SC2Assets"
                << "SC2Replay" << "scx" << "w3n" << "snp" << "sv" << "hsv";
    setValue("extensions", extensions);
}

QVariant MPQExtensionsPage::value(const QString & key)
{
    if (key == "extensions")
        return MPQSettings::instance()->MPQExtensions();
    return QVariant();
}

void MPQExtensionsPage::setValue(const QString & key, const QVariant & value)
{
	if (key == "extensions") {
        QStringList values = value.toStringList();
        if (!values.isEmpty()) {
        MPQSettings::instance()->setMPQExtensions(values);
        manager()->removePrefix("mpq:");
        manager()->addSuffixes(values, "mpq:");
        } else {
            setDefaults();
        }
	}
    m_widget->init();
}
