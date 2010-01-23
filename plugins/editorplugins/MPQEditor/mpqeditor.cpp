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

#include <QDebug>

QDirModel * MPQEditor::m_model = 0;

MPQEditor::MPQEditor(QWidget *parent) :
    QWidget(parent),
    listView(new QListView),
    iconView(new QListView),
    columnView(new QColumnView(this)),
    tableView(new QTableView(this)),
    treeView(new QTreeView(this)),
    currentView(0)
{
//    QHBoxLayout * layout = new QHBoxLayout(this);
    layout = new QStackedLayout(this);
    views[ListView] = listView;
    views[IconView] = iconView;
    views[TableView] = tableView;
    views[ColumnView] = columnView;
    views[TreeView] = treeView;

    for (int i = 0; i < MaxViews; i++)
        layout->addWidget(views[i]);
    layout->setContentsMargins(0, 0, 0, 0);
//    layout->setMargin(0);
    setLayout(layout);

    listView->setUniformItemSizes(true);
    iconView->setUniformItemSizes(true);
    iconView->setViewMode(QListView::IconMode);
    iconView->setSpacing(24);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    if (!m_model) {
        m_model = new QDirModel;
        m_model->setReadOnly(false);
        m_model->setSupportedDragActions(Qt::CopyAction | Qt::MoveAction);
        m_model->setSorting(QDir::DirsFirst);
    }
    for (int i = 0; i < MaxViews; i++) {
        views[i]->setEditTriggers(QAbstractItemView::SelectedClicked);
        views[i]->setModel(m_model);
//        views[i]->setDragDropMode(QAbstractItemView::DragDrop);
        views[i]->setDragDropMode(QAbstractItemView::DropOnly);
//        views[i]->setDragEnabled(true);
        views[i]->setAcceptDrops(true);
        views[i]->setDragDropOverwriteMode(false);
        views[i]->setDefaultDropAction(Qt::MoveAction);
        connect(views[i], SIGNAL(doubleClicked(const QModelIndex &)), SLOT(onDoubleClick(const QModelIndex &)));
//fixes slow initializing of view under windows
#warning TODO: remove after writing new model
        views[i]->setRootIndex(m_model->index(QDesktopServices::storageLocation(QDesktopServices::HomeLocation)));
    }

    setViewMode(ListView);
}

MPQEditor::~MPQEditor()
{
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
//    qDebug() << "MPQEditor::setViewMode" << mode;
    layout->setCurrentIndex(mode);

    m_viewMode = mode;
    currentView = views[mode];

    if (mode < 3)
        views[mode]->setRootIndex(currentView ? currentView->rootIndex() : QModelIndex() ); //sets the same directory for list and table views
    else
        views[mode]->setRootIndex(m_model->index(m_currentFile));

//    setFocusPolicy(Qt::NoFocus);
//    open(m_currentFile);
}

void MPQEditor::open(const QString &file)
{
    if (m_currentFile != file) {
        m_currentFile = file;
        currentView->setRootIndex(m_model->index(file));
        emit currentChanged(file);
    }
}

void MPQEditor::closeFile()
{
    m_currentFile = "";
    if (isVisible())
        currentView->setRootIndex(QModelIndex());
}

QString MPQEditor::selectedDir()
{
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
        QMessageBox box(QMessageBox::Information, "Warning", "Select exactly one file or folder", QMessageBox::Ok);
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
                QMessageBox box(QMessageBox::Critical, "Critical Error", "Can't add files: "+ targetFile.errorString(), QMessageBox::Ok, this);
                box.exec();
                return;
            }
        }
    }
}

void MPQEditor::extract(const QString & destDir)
{
    foreach (QModelIndex index, selectedIndexes()) {
//        index = proxy->mapToSource(index);
        QFile file(m_model->filePath(index));
        QFileInfo info(m_model->filePath(index));
//        qDebug() << file.fileName();
        bool result = file.copy(destDir + "/" + info.fileName());
//        bool result = model->extract(index, path);
        if (!result) {
            QMessageBox box(QMessageBox::Critical, "Critical Error", "Can't extract file "
                            + index.data(Qt::DisplayRole).toString() + ": "+ file.errorString(), QMessageBox::Ok);
            box.exec();
            return;
        }
    }
}

void MPQEditor::remove()
{
    foreach (QModelIndex index, selectedIndexes()) {
//        QFile file(m_model->filePath(index));
        bool result;
        QFileInfo info(m_model->filePath(index));
        if (info.isDir())
            result = m_model->rmdir(index);
        else
            result = m_model->remove(index);
        if (!result) {
            QMessageBox box(QMessageBox::Critical, "Critical Error", "Can't remove file "
                            + index.data(Qt::DisplayRole).toString() + ": "/*+ file.errorString()*/, QMessageBox::Ok);
            box.exec();
            return;
        }
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

void MPQEditor::up()
{
    if (canUp())
        currentView->setRootIndex(currentView->rootIndex().parent());
}

void MPQEditor::newFolder(const QString & name)
{
    QString dir = selectedDir();
    if (dir == "") {
//        emit error;
    } else {
        QString folderName = name;
        if (folderName == "")
            folderName = "New Folder";
        QModelIndex index = m_model->mkdir(m_model->index(dir), folderName);

#warning TODO: remove if condition when Trolls fix bug with QColumnView crash
        if (m_viewMode != ColumnView /*index.isValid()*/)
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
//        emit openRequested(m_model->filePath(index));
        emit currentChanged(m_model->filePath(index));
    }
}
