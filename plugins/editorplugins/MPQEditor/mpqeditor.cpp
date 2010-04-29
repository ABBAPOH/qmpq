#include "mpqeditor.h"

#include <QtGui/QListView>
#include <QtGui/QColumnView>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>
#include <QtGui/QHBoxLayout>
//#include <QtGui/QDirModel>
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
#include "../../../../3rdParty/FileSystemModel/filesystemmodel.h"

#include "universalview.h"
#include "idirmodel.h"
#include "archivesuffixesmanager.h"

//QDirModel * MPQEditor::m_model = 0;

MPQEditor::MPQEditor(QWidget *parent) :
    QWidget(parent),
    m_view(new UniversalView(this)),
    m_layout(new QHBoxLayout(this)),
    m_model(0)
{
    initModel();
    initViews();

    setViewMode(UniversalView::ListView);

    initActions();

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_view);
    setLayout(m_layout);

    suffixesManager = new ArchiveSuffixesManager;
    QStringList suffixes;
    suffixes << "mpq" << "w3x" << "w3m" << "s2ma" << "SC2Data" << "SC2Archive" << "SC2Assets"
            << "SC2Replay" << "scx" << "w3n" << "snp" << "sv" << "hsv";
    suffixesManager->addSuffixes(suffixes, "mpq:");
}

MPQEditor::~MPQEditor()
{
    MPQEditorError error;
}

void MPQEditor::initModel()
{
//    if (!m_model) {
////        m_model = new QDirModel;
//        m_model = new FileSystemModel;
//        m_model->setReadOnly(false);
//        m_model->setSupportedDragActions(Qt::CopyAction | Qt::MoveAction);
//        m_model->Path("");
////        m_model->setSorting(QDir::DirsFirst);
//    }
}

bool canModelHandle(const IDirModel * model, const QModelIndex & index)
{
    return index.isValid() && model->isDir(index);
}

void MPQEditor::initModel(const QString & path)
{
    QModelIndex index;
    if (m_model)
        index = m_model->index(path);
//    if (!canModelHandle(m_model, index))
    {
        // we have to try to switch model
        delete m_model;
        m_model = 0;
        if (path.startsWith("mpq:"))
            m_model = new DirModelWrapper(path);
        if (path.startsWith(":/"))
            m_model = new DirModelWrapper(path);
        if (!m_model)
            m_model = new FileSystemModelWrapper;
        m_view->setModel(m_model->model());
        qDebug() << m_model->model()->metaObject()->className();
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
    m_view->setEditTriggers(QAbstractItemView::SelectedClicked);
    m_view->setDragDropMode(QAbstractItemView::DropOnly);
    m_view->setAcceptDrops(true);
    m_view->setDragDropOverwriteMode(false);
    m_view->setDefaultDropAction(Qt::MoveAction);
    m_view->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(m_view, SIGNAL(doubleClicked(QModelIndex)), SLOT(onDoubleClick(QModelIndex))), Qt::QueuedConnection;
}

QModelIndexList MPQEditor::selectedIndexes()
{
    if (m_view->viewMode() == UniversalView::ListView || m_view->viewMode() == UniversalView::ColumnView)
        return m_view->selectionModel()->selectedIndexes();
    else
        return m_view->selectionModel()->selectedRows();
}

void MPQEditor::setViewMode(UniversalView::ViewMode mode)
{
    m_view->setViewMode(mode);
}

void MPQEditor::open(const QString &file)
{
    if (m_currentFile != file) {
        m_currentFile = file;
        initModel(file);
        m_view->setRootIndex(m_model->index(file));
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
//            currentView->update(m_model->index(archivePath));
//            m_model->update(archivePath);
        }
    }
}

void MPQEditor::closeFile()
{
    m_currentFile = "";
//    if (isVisible())
//        currentView->Index(QModelIndex());
//    m_model->refresh(QModelIndex());
}

QString MPQEditor::selectedDir()
{
//    if (m_viewMode != ColumnView)
//        return m_currentFile;
//
//    QModelIndexList list = selectedIndexes();
//    QString result;
//    if (list.count() == 0) {
//        result =  m_currentFile;
//    } else if (list.count() == 1) {
//        result =  m_model->filePath(list.first());
//    } else
//        return "";
//    QFileInfo info(result);
//    if (!info.isDir())
//        result = info.path();
//
//    return result;
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

//            m_model->refresh(m_model->index(info.absolutePath()));
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
//qDebug() << m_model->filePath(index);
//
//    if (m_model->isDir(index)) { // recursively removes current dir
//        QDir dir(m_model->filePath(index));
//        foreach (QString child, dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot)) {
//            remove(m_model->index(dir.filePath(child)));
//        }
//
////        for (int row = 0, rows = m_model->rowCount(index); row < rows; row++) {
////            remove(index.child(0, 0));
////        }
////        m_model->in
////        QApplication::processEvents();
//        bool result = m_model->rmdir(index);
//        qDebug() << result;
//    } else { // removes current file
//        bool result = m_model->remove(index);
//    }
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
        m_view->edit(indexes.first());
    }
}

bool MPQEditor::canUp()
{
//    return m_model->filePath(currentView->rootIndex().parent()).startsWith(m_currentFile);
}

//QAbstractItemModel * MPQEditor::model()
//{
//    return m_model;
//}

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
//    if (show) {
//        treeView->setColumnHidden(1, false);
//        treeView->setColumnHidden(2, false);
//        treeView->setColumnHidden(3, false);
//        tableView->setColumnHidden(1, false);
//        tableView->setColumnHidden(2, false);
//        tableView->setColumnHidden(3, false);
//    } else {
//        treeView->setColumnHidden(1, true);
//        treeView->setColumnHidden(2, true);
//        treeView->setColumnHidden(3, true);
//        tableView->setColumnHidden(1, true);
//        tableView->setColumnHidden(2, true);
//        tableView->setColumnHidden(3, true);
//    }
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

bool MPQEditor::isMPQArchive(const QString & file)
{
    QMPQFileEngine * engine = dynamic_cast<QMPQFileEngine*>(QFile(file).fileEngine());
    if (engine) {
        qDebug("archive!");
        return true;
    }
    return false;
}


void MPQEditor::up()
{
    if (canUp())
        m_view->setRootIndex(m_view->rootIndex().parent());
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
            m_view->edit(index);
    }
}

void MPQEditor::onDoubleClick(const QModelIndex & index)
{
//    qDebug() << "MPQEditor::onDoubleClick";
    qDebug() << m_model->filePath(index);
    QString path = m_model->filePath(index);
    QFileInfo info(path);
    if (info.isDir()/* && m_viewMode < 3*/) {
        open(path);
//        emit currentChanged(m_model->filePath(index));
    } else {
        QString prefix = suffixesManager->mapFromPath(path);
        if (prefix != "")
            open(prefix + path);
        else
            emit openRequested(path);
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

