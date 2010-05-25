#include "mpqarchiveerrorhandler.h"

#include "../QMPQFileEngine/qmpqarchive.h"
#include "../QMPQFileEngine/qmpqarchivecache.h"

ErrorsDialog::ErrorsDialog(QWidget * parent) :
        QDialog(parent),
        ui(new Ui::ErrorsDialog)
{
    ui->setupUi(this);
}

ErrorsDialog::~ErrorsDialog()
{
    delete ui;
}

void ErrorsDialog::clearErrors()
{
//    ui->treeWidget->§
    while (QTreeWidgetItem * item = ui->treeWidget->takeTopLevelItem(0)) {
        delete item;
    }
}

void ErrorsDialog::addError(const QString & errString)
{
    QTreeWidgetItem * item = new QTreeWidgetItem();
    item->setData(0, Qt::DisplayRole, errString);
    ui->treeWidget->addTopLevelItem(item);
}

MPQArchiveErrorHandler::MPQArchiveErrorHandler(QObject *parent) :
    QObject(parent), dialog(new ErrorsDialog)

{
    QMPQArchiveCache * cache = QMPQArchiveCache::instance();
    connect(cache, SIGNAL(objectCreated(QObject*)), SLOT(addObject(QObject*)));
}

MPQArchiveErrorHandler::~MPQArchiveErrorHandler()
{
    delete dialog;
}

void MPQArchiveErrorHandler::onError(QMPQArchive * archive)
{
    if (!dialog->isVisible()) {
        dialog->clearErrors();
    }
    dialog->addError(archive->errorString());
    dialog->show();
    qApp->processEvents();
}

void MPQArchiveErrorHandler::onError()
{
    QMPQArchive * archive = qobject_cast<QMPQArchive *>(sender());
    Q_ASSERT(archive);
    onError(archive);
}

void MPQArchiveErrorHandler::addObject(QObject * object)
{
    QMPQArchive * archive = qobject_cast<QMPQArchive *>(object);
    Q_ASSERT(archive);
    if (archive->lastError() != QMPQArchive::NoError) {
        onError(archive);
    }
    connect(archive, SIGNAL(error()), SLOT(onError()));
}
