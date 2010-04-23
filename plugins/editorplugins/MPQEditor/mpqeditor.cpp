#include "mpqeditor.h"

#include <QtGui/QListView>
#include <QtGui/QColumnView>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>
#include <QtGui/QStackedLayout>
#include <QtGui/QDirModel>
#include <QtGui/QApplication>
#include <QtCore/QAbstractItemModel>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>
#include <QtGui/QAction>
#include <QtGui/QMenu>

#include <QDebug>

#include <icore.h>

#include "../../../../QMPQFileEngine/qmpqfileengine.h"
#include "../../../../QMPQFileEngine/qmpqarchive.h"

QDirModel * MPQEditor::m_model = 0;

MPQEditor::MPQEditor(QWidget *parent) :
    QWidget(parent),
    listView(new QListView),
    iconView(new QListView),
    columnView(new QColumnView(this)),
    tableView(new QTableView(this)),
    treeView(new QTreeView(this)),
    currentView(0),
    layout(new QStackedLayout(this))
{
    initModel();
    initViews();

    setViewMode(ListView);

    initActions();
}

MPQEditor::~MPQEditor()
{
    MPQEditorError error;
}

void MPQEditor::initModel()
{
    if (!m_model) {
        m_model = new QDirModel;
        m_model->setReadOnly(false);
        m_model->setSupportedDragActions(Qt::CopyAction | Qt::MoveAction);
        m_model->setSorting(QDir::DirsFirst);
    }
}

void MPQEditor::initActions()
{
//    openAction = new QAction("open", this);
//#ifdef Q_OS_WIN
//    openAction->setShortcut(tr("Return"));
//#endif
//#ifdef Q_OS_MAC
//    openAction->setShortcut(tr("Ctrl+O"));
//#endif
//    openAction->setShortcutContext(Qt::ApplicationShortcut);
//    connect(openAction, SIGNAL(triggered()), SLOT(onOpenRequest()));
//    addAction(openAction);
}

void MPQEditor::initViews()
{
    views[ListView] = listView;
    views[IconView] = iconView;
    views[TableView] = tableView;
    views[ColumnView] = columnView;
    views[TreeView] = treeView;

    for (int i = 0; i < MaxViews; i++)
        layout->addWidget(views[i]);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    listView->setUniformItemSizes(true);
    iconView->setUniformItemSizes(true);
    iconView->setViewMode(QListView::IconMode);
    iconView->setSpacing(24);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    for (int i = 0; i < MaxViews; i++) {
        views[i]->setEditTriggers(QAbstractItemView::SelectedClicked);
        views[i]->setModel(m_model);
//        views[i]->setDragDropMode(QAbstractItemView::DragDrop);
        views[i]->setDragDropMode(QAbstractItemView::DropOnly);
//        views[i]->setDragEnabled(true);
        views[i]->setAcceptDrops(true);
        views[i]->setDragDropOverwriteMode(false);
        views[i]->setDefaultDropAction(Qt::MoveAction);
        views[i]->setSelectionMode(QAbstractItemView::ExtendedSelection);
        connect(views[i], SIGNAL(doubleClicked(const QModelIndex &)), SLOT(onDoubleClick(const QModelIndex &)));
//fixes slow initializing of view under windows
#warning TODO: remove after writing new model
        views[i]->setRootIndex(m_model->index(QDesktopServices::storageLocation(QDesktopServices::HomeLocation)));
    }
    tableView->setColumnWidth(0, 300);
    tableView->setColumnWidth(3, 125);
    treeView->setColumnWidth(0, 300);
}

QModelIndexList MPQEditor::selectedIndexes()
{
    if (currentView == listView || currentView == columnView)
        return currentView->selectionModel()->selectedIndexes();
    else
        return currentView->selectionModel()->selectedRows();
}

void MPQEditor::setViewMode(ViewMode mode)
{
    layout->setCurrentIndex(mode);

    if (mode < 3)
        views[mode]->setRootIndex(currentView ? currentView->rootIndex() : QModelIndex() ); //sets the same directory for list and table views
    else
        views[mode]->setRootIndex(m_model->index(m_currentFile));

    m_viewMode = mode;
    currentView = views[mode];
}

void MPQEditor::open(const QString &file)
{
    if (m_currentFile != file) {
        m_currentFile = file;
        currentView->setRootIndex(m_model->index(file));
        emit currentChanged(file);
    }
}

void MPQEditor::reopenUsingListfile(const QByteArray &listfile)
{
    QModelIndexList list = selectedIndexes();
    if (list.isEmpty())
        return;
    QString filePath = m_model->filePath(list.first());
    QFile file(filePath);
    //  if we can cat file then we're in mpq archive
    QMPQFileEngine * engine = dynamic_cast<QMPQFileEngine *>(file.fileEngine());
    if (engine) {
        QMPQArchive * archive = engine->archive();
        if (archive) {
            QString archivePath = archive->file();
            archive->closeArchive();
            archive->openArchive(archivePath, listfile);
            currentView->update(m_model->index(archivePath));

        }
    }
}

void MPQEditor::closeFile()
{
    m_currentFile = "";
    if (isVisible())
        currentView->setRootIndex(QModelIndex());
    m_model->refresh(QModelIndex());
}

QString MPQEditor::selectedDir()
{
    if (m_viewMode != ColumnView)
        return m_currentFile;

    QModelIndexList list = selectedIndexes();
    QString result;
    if (list.count() == 0) {
        result =  m_currentFile;
    } else if (list.count() == 1) {
        result =  m_model->filePath(list.first());
    } else
        return "";
    QFileInfo info(result);
    if (!info.isDir())
        result = info.path();

    return result;
}

void MPQEditor::add(const QStringList & files)
{
    if (files.isEmpty())
        return;
    QString dir = selectedDir();
//    QModelIndexList list = selectedIndexes();
    if (dir == "") {
        QMessageBox box(QMessageBox::Information, tr("Warning"), tr("Select exactly one file or folder"), QMessageBox::Ok);
        box.exec();
        return;
    } else {
//        model->add(files, list.count() == 0 ? QModelIndex() : list.first());
//        bool result = true;
        foreach (QString filePath, files) {
            QFile file(filePath);
            QFileInfo fileInfo(filePath);
            QFileInfo info(dir);

            QString newPath = info.absoluteFilePath() + "/" + fileInfo.fileName();
//            result = file.copy(newPath);
            file.open(QFile::ReadOnly);
            QFile targetFile(newPath);
            targetFile.open(QFile::WriteOnly);
            QByteArray arr = file.readAll();
//                qDebug() << arr.size();
//                qDebug() << targetFile.write(arr);
            bool result = targetFile.write(arr) == arr.size();
            file.close();
            targetFile.close();

            m_model->refresh(m_model->index(info.absolutePath()));
            if (!result) {
                QMessageBox box(QMessageBox::Critical, tr("Critical Error"), tr("Can't add files: ") + targetFile.errorString(), QMessageBox::Ok, this);
                box.exec();
                return;
            }
        }
    }
}

void MPQEditor::extract(const QString & path, const QString & dest)
{
    QFileInfo info(path);
    QDir destDir(dest);
    if (info.isDir()) { // recursively copies current dir
        QDir dir(path);
        destDir.mkdir(dir.dirName());
        foreach (QString child, dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot)) {
            extract(dir.filePath(child), destDir.filePath(dir.dirName()));
        }
    } else { // copies current file
        QFile file(path);
        bool result = file.copy(destDir.filePath(info.fileName()));
    }
}

void MPQEditor::extract(const QString & destDir)
{
    foreach (QModelIndex index, selectedIndexes()) {
//        index = proxy->mapToSource(index);
        extract(m_model->filePath(index), destDir);
        continue;
        QFile file(m_model->filePath(index));
        QFileInfo info(m_model->filePath(index));
//        qDebug() << file.fileName();
        bool result = file.copy(destDir + "/" + info.fileName());
//        bool result = model->extract(index, path);
        if (!result) {
            QMessageBox box(QMessageBox::Critical, tr("Critical Error"), tr("Can't extract file ")
                            + index.data(Qt::DisplayRole).toString() + ": "+ file.errorString(), QMessageBox::Ok);
            box.exec();
            return;
        }
    }
}

void MPQEditor::remove(const QModelIndex & index)
{
qDebug() << m_model->filePath(index);

    if (m_model->isDir(index)) { // recursively removes current dir
        QDir dir(m_model->filePath(index));
        foreach (QString child, dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot)) {
            remove(m_model->index(dir.filePath(child)));
        }

//        for (int row = 0, rows = m_model->rowCount(index); row < rows; row++) {
//            remove(index.child(0, 0));
//        }
//        m_model->in
//        QApplication::processEvents();
        bool result = m_model->rmdir(index);
        qDebug() << result;
    } else { // removes current file
        bool result = m_model->remove(index);
    }
}

void MPQEditor::remove()
{
    foreach (QModelIndex index, selectedIndexes()) {
//        QFile file(m_model->filePath(index));
        bool result;
        remove(index);
    }
}

void MPQEditor::rename()
{
    QModelIndexList indexes = selectedIndexes();
    if (indexes.count() != 1) {
//        emit error();
    } else {
        currentView->edit(indexes.first());
    }
}

bool MPQEditor::canUp()
{
    return m_model->filePath(currentView->rootIndex().parent()).startsWith(m_currentFile);
}

QAbstractItemModel * MPQEditor::model()
{
    return m_model;
}

QStringList MPQEditor::selectedPaths()
{
    QStringList result;
    QModelIndexList list = selectedIndexes();

    foreach (QModelIndex index, list) {
        result.append(m_model->filePath(index));
    }
    return result;
}

void MPQEditor::showColumns(bool show)
{
    if (show) {
        treeView->setColumnHidden(1, false);
        treeView->setColumnHidden(2, false);
        treeView->setColumnHidden(3, false);
        tableView->setColumnHidden(1, false);
        tableView->setColumnHidden(2, false);
        tableView->setColumnHidden(3, false);
    } else {
        treeView->setColumnHidden(1, true);
        treeView->setColumnHidden(2, true);
        treeView->setColumnHidden(3, true);
        tableView->setColumnHidden(1, true);
        tableView->setColumnHidden(2, true);
        tableView->setColumnHidden(3, true);
    }
}

#warning do not use
QMPQFileEngine * MPQEditor::getIndexMPQEngine(const QModelIndex & index)
{
//    QString path = m_model->filePath(index);
//    QFile file(path);
//    //  if we can cat file then we're in mpq archive
//    return dynamic_cast<QMPQFileEngine *>(file.fileEngine());
}

bool MPQEditor::isMPQArchive()
{
    QModelIndexList list = selectedIndexes();
    if (list.isEmpty())
        return false;
    //  if we can cat file then we're in mpq archive
    QMPQFileEngine * engine = getIndexMPQEngine(list.first());
    if (engine) {
        qDebug("archive!");
        return true;
    }
    return engine;
}

void MPQEditor::up()
{
    if (canUp())
        currentView->setRootIndex(currentView->rootIndex().parent());
}

void MPQEditor::newFolder(const QString & name)
{
    QString dir = selectedDir();
    qDebug() << dir;
    if (dir == "") {
//        emit error;
    } else {
        QString folderName = name;
        if (folderName == "")
            folderName = tr("New Folder");
        QModelIndex index = m_model->mkdir(m_model->index(dir), folderName);

        if (index.isValid())
            currentView->edit(index);
    }
}

void MPQEditor::onDoubleClick(const QModelIndex & index)
{
//    qDebug() << "MPQEditor::onDoubleClick";
    if (QFileInfo(m_model->filePath(index)).isDir()/* && m_viewMode < 3*/) {
        open(m_model->filePath(index));
//        emit currentChanged(m_model->filePath(index));
    } else {
        emit openRequested(m_model->filePath(index));
//        emit currentChanged(m_model->filePath(index));
    }
}

void MPQEditor::onOpenRequest()
{
    QModelIndexList list = selectedIndexes();
    if (list.isEmpty())
        return;
    emit openRequested(m_model->filePath(list.first()));
}

