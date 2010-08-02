#include "mpqeditor.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QAction>
#include <QtGui/QClipboard>
#include <QtGui/QKeyEvent>

#include <QDebug>

#include <icore.h>

#include <qmpqfileengine.h>
#include <qmpqarchiveex.h>

#include "universalview.h"
#include "idirmodel.h"

MPQEditor::MPQEditor(QWidget *parent) :
    QWidget(parent),
    m_view(new UniversalView(this)),
    m_layout(new QHBoxLayout(this)),
    m_model(0)
{
    initViews();

    setViewMode(UniversalView::ListView);

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_view);
    setLayout(m_layout);
    setFocusPolicy(Qt::StrongFocus);
}

MPQEditor::~MPQEditor()
{
    MPQEditorError error;
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
    //  if old path is not part of a new path, it is a chance that we have to switch archive
    if ( (currentType != newType || m_model == 0) || (!path.startsWith(m_currentFile)) )
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

void MPQEditor::initViews()
{
    m_view->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
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
        emit opened(file);
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
        QMPQArchiveEx * archive = engine->archive();
        if (archive) {
            QString archivePath = archive->file();
            archive->close();
            archive->open(archivePath, 0, listfile);
//            currentView->update(m_model->index(archivePath));
//            m_model->update(archivePath);
            MyDirModel * model = qobject_cast<MyDirModel *>(m_model->model());
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

        //  we manually refresh model if it is MyDirModel or it's subclass
        MyDirModel * model = qobject_cast<MyDirModel *>(m_model->model());
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
    if (index.isValid()) {
        m_model->remove(index);
    }
}

void MPQEditor::remove()
{
    QModelIndexList indexes = selectedIndexes();
//    qSort(indexes.begin(), indexes.end());

//    QString lastFile = m_model->filePath(indexes.last());
    QStringList files;
    for(int i = indexes.count() - 1; i > -1; --i) {
        files.append(m_model->filePath(indexes.at(i)));
    }
    foreach(QString file, files) {
        QModelIndex index = m_model->index(file, 0);
        remove(index);
    }

//    QItemSelectionModel * model = m_view->currentView()->selectionModel();
//    model->clearSelection();
//    model->select(m_model->index(lastFile, 0), QItemSelectionModel::Select);
//    m_view->currentView()->setSelectionModel(model);
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

void MPQEditor::refresh(const QString & path)
{
    //  we manually refresh model if it is MyDirModel or it's subclass
    MyDirModel * model = qobject_cast<MyDirModel *>(m_model->model());
    if (model)
        model->refresh(m_model->index(path));
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

bool MPQEditor::isMPQArchive(const QString & file)
{
    QMPQFileEngine * engine = dynamic_cast<QMPQFileEngine*>(QFile(file).fileEngine());
    if (engine) {
        return true;
    }
    return false;
}

void MPQEditor::newFolder(const QString & name)
{
    QString dir = selectedDir();
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

void MPQEditor::setNameFilter(const QString & filter)
{
    m_model->setNameFilters(QStringList() << currentFile().split('/') << filter);
}

void MPQEditor::copy()
{
    QClipboard * clipboard = QApplication::clipboard();
    QMimeData * data = new QMimeData();
    QList<QUrl> urls;
    foreach (QString path, selectedPaths()) {
        urls.append(QUrl::fromLocalFile(path));
    }
    data->setUrls(urls);
    clipboard->setMimeData(data);
}

void MPQEditor::paste()
{
    QClipboard * clipboard = QApplication::clipboard();
    const QMimeData * data = clipboard->mimeData();
    QList<QUrl> urls = data->urls();
    QDir dir(currentFile());
    foreach (QUrl path, urls) {
        QString filePath = path.toLocalFile();
        QFile file(filePath);
        QFileInfo info(filePath);
        QString targetPath = dir.absolutePath() + '/' + info.fileName();
        if (dir == info.dir()) {
            int i = 0;
            do  {
                i++;
                targetPath = dir.absolutePath() + '/' + info.baseName() + ' ' +
                             QString::number(i);
                if (info.suffix() != "") {
                    targetPath += '.' + info.suffix();
                }
            } while (QFile::exists(targetPath));
            file.copy(targetPath);
        } else {
            file.copy(targetPath);
        }
    }
    this->m_model->refresh(m_model->index(currentFile()));
}

void MPQEditor::onDoubleClick(const QModelIndex & index)
{
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

void MPQEditor::keyPressEvent ( QKeyEvent * event )
{
    if (!sendingEvent) {
    sendingEvent = true;
    qApp->sendEvent(m_view->currentView(), event);
    sendingEvent = false;
}
}
