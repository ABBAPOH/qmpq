#include "mpqeditor.h"

#include <QtGui/QListView>
#include <QtGui/QColumnView>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDirModel>
#include <QtGui/QApplication>
#include <QtCore/QAbstractItemModel>
#include <QtGui/QMessageBox>
#include <QDebug>

QDirModel * MPQEditor::m_model = 0;

MPQEditor::MPQEditor(QWidget *parent) :
    QWidget(parent),
    listView(new QListView(this)),
    columnView(new QColumnView(this)),
    tableView(new QTableView(this)),
    treeView(new QTreeView(this)),
    currentView(0)
{
    QHBoxLayout * layout = new QHBoxLayout(this);
    views[0] = listView;
    views[1] = listView;
    views[2] = tableView;
    views[3] = columnView;
    views[4] = treeView;
    for(int i = 0; i < MaxViews; i++)
        layout->addWidget(views[i]);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setMargin(0);
    setLayout(layout);

    listView->setUniformItemSizes(true);

    if (!m_model) {
        m_model = new QDirModel;
        m_model->setReadOnly(false);
        m_model->setSupportedDragActions(Qt::CopyAction | Qt::MoveAction);
    }
    for (int i = 0; i < MaxViews; i++) {
        views[i]->setModel(m_model);
        views[i]->setDragDropMode(QAbstractItemView::DragDrop);
        views[i]->setDragEnabled(true);
        views[i]->setAcceptDrops(true);
//        views[i]->setDragDropOverwriteMode(false);
//        views[i]->setDefaultDropAction(Qt::MoveAction);
//        views[i]->setDropIndicatorShown(true);
        connect(views[i], SIGNAL(doubleClicked(const QModelIndex &)), SLOT(onDoubleClick(const QModelIndex &)));
    }

    setViewMode(ListView);
//    listView->setModel(model);
//    columnView->setModel(model);
//    treeView->setModel(model);
//    treeView->setModel(model);
}

void MPQEditor::setViewMode(ViewMode mode)
{
//    qDebug() << "MPQEditor::setViewMode" << mode;
    m_viewMode = mode;
    for (int i = 0; i < MaxViews; i++)
        views[i]->hide();
    if (mode == 1)
        listView->setViewMode(QListView::IconMode);
    if (mode == 0)
        listView->setViewMode(QListView::ListMode);

    if (mode < 3)
        views[mode]->setRootIndex(currentView ? currentView->rootIndex() : QModelIndex() ); //sets the same directory for list and table views
    else
        views[mode]->setRootIndex(m_model->index(m_currentFile));
    currentView = views[mode];
    currentView->show();
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
    currentView->setRootIndex(QModelIndex());
}

void MPQEditor::add(const QStringList & files)
{
    if (files.isEmpty())
        return;
    QModelIndexList list = currentView->selectionModel()->selectedRows();
    if (list.count() != 1) {
        QMessageBox box(QMessageBox::Information, "Warning", "Select exactly one file or folder", QMessageBox::Ok);
        box.exec();
        return;
    } else {
//        model->add(files, list.count() == 0 ? QModelIndex() : list.first());
//        bool result = true;
        foreach (QString filePath, files) {
            QFile file(filePath);
            QFileInfo fileInfo(filePath);
            QFileInfo info(m_model->filePath(list.first()));
            if (!info.isDir())
                info = QFileInfo(info.dir().absolutePath());

            QString newPath = info.absolutePath() + "/" + fileInfo.fileName();
            qDebug() << newPath;
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
    foreach (QModelIndex index, currentView->selectionModel()->selectedRows()) {
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
    foreach (QModelIndex index, currentView->selectionModel()->selectedRows()) {
//        QFile file(model->filePath(index));
        bool result = m_model->remove(index);
        if (!result) {
            QMessageBox box(QMessageBox::Critical, "Critical Error", "Can't extract file "
                            + index.data(Qt::DisplayRole).toString() + ": "/*+ file.errorString()*/, QMessageBox::Ok);
            box.exec();
            return;
        }
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

void MPQEditor::onDoubleClick(const QModelIndex & index)
{
    qDebug() << "MPQEditor::onDoubleClick";
    if (QFileInfo(m_model->filePath(index)).isDir()/* && m_viewMode < 3*/) {
        qDebug() << "dir";
        open(m_model->filePath(index));
//        currentView->setRootIndex(index);
//        emit currentChanged(m_model->filePath(index));
    } else {
        qDebug() << m_model->filePath(index);
//        emit openRequested(m_model->filePath(index));
        emit currentChanged(m_model->filePath(index));
    }
}


