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

MPQEditor::ModelType MPQEditor::getModelType(const QString & path)
{
    if (path.startsWith("mpq:"))
        return DirModel;
    if (path.startsWith(":"))
        return DirModel;
    else
        return FileSystemModel;
}

bool canModelHandle(const IDirModel * model, const QModelIndex & index)
{
    return index.isValid() && model->isDir(index);
}

void MPQEditor::initModel(const QString & path)
{
    // we have to try to switch model
    ModelType currentType = getModelType(m_currentFile);
    ModelType newType = getModelType(path);
    qDebug() << path << currentType << newType;
    if (currentType != newType || m_model == 0)
    {
        delete m_model;
        m_model = 0;
        if (newType == DirModel)
            m_model = new DirModelWrapper(path);
        else if (newType == FileSystemModel)
            m_model = new FileSystemModelWrapper;
        m_view->setModel(m_model->model());
        resizeColumns();
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
//    m_view->setDragDropMode(QAbstractItemView::DropOnly);
    m_view->setDragDropMode(QAbstractItemView::DragDrop);
    m_view->setAcceptDrops(true);
//    m_view->setDragDropOverwriteMode(false);
//    m_view->setDefaultDropAction(Qt::MoveAction);
    m_view->setSelectionMode(QAbstractItemView::ExtendedSelection);

    #warning TODO: see why signal emited before some actions
    connect(m_view, SIGNAL(doubleClicked(QModelIndex)), SLOT(onDoubleClick(QModelIndex)), Qt::QueuedConnection);
}

void MPQEditor::resizeColumns()
{
    m_view->setColumnWidth(0, 300);
    m_view->setColumnWidth(1, 100);
    m_view->setColumnWidth(2, 100);
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
        initModel(file);
        m_currentFile = file;
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
            QDirModel * model = qobject_cast<QDirModel *>(m_model->model());
            if (model)
                model->refresh(model->index(archivePath));
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
    if (m_view->viewMode() != UniversalView::ColumnView)
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
    bool ok = true;

    if (files.isEmpty())
        return;
    QString dir = selectedDir();
    m_lastError = MPQEditorError();

    foreach (QString filePath, files) {
        QFile file(filePath);
        QFileInfo fileInfo(filePath);
        QFileInfo info(dir);

        QString newPath = info.absoluteFilePath() + "/" + fileInfo.fileName();
        bool result = file.copy(newPath);
        ok |= result;

        //  we manually refresh model if it is QDirModel or it's subclass
        QDirModel * model = qobject_cast<QDirModel *>(m_model->model());
        if (model)
            model->refresh(m_model->index(info.absolutePath()));
        if (!result) {
            m_lastError.addSubError(MPQEditorError(file.errorString()));
        }
    }

    if (!ok) {
        m_lastError.setErrorString("Error while adding files:");
        emit errorOccured(m_lastError);
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
        if (!result) {
            m_lastError.addSubError(MPQEditorError(file.errorString()));
        }
    }
}

/*
  \func void MPQEditor::extract(const QString & destDir)
    extracts selected files and folders to \a destDir
*/
void MPQEditor::extract(const QString & destDir)
{
    m_lastError = MPQEditorError();

    foreach (QModelIndex index, selectedIndexes()) {
        extract(m_model->filePath(index), destDir);
    }

    if (!m_lastError.subErrors().isEmpty()) {
        m_lastError.setErrorString("Error while extracting files:");
        emit errorOccured(m_lastError);
    }
}

void MPQEditor::remove(const QModelIndex & index)
{
    m_model->remove(index);
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
        emit currentChanged(path);
    } else {
        emit openRequested(path);
    }
}

void MPQEditor::onOpenRequest()
{
    QModelIndexList list = selectedIndexes();
    if (list.isEmpty())
        return;
    emit openRequested(m_model->filePath(list.first()));
}

