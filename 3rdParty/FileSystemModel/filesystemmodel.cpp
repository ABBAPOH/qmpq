#include "filesystemmodel.h"

#include <private/qfilesystemmodel_p.h>
//#include <QDebug>

FileSystemModel::FileSystemModel(QObject *parent) :
    QFileSystemModel(parent)
{
}

bool FileSystemModel::rmdir(const QModelIndex &aindex) const
{
//    Q_D(const QFileSystemModel);
//    reinterpret_cast<QFileSystemModelPrivate*>(qGetPtrHelper(d_ptr))->fileInfoGatherer.updateFile(filePath(aindex));
    update(filePath(aindex.parent()));
    return QFileSystemModel::rmdir(aindex);
}

bool FileSystemModel::remove(const QModelIndex &aindex) const
{
    update(filePath(aindex.parent()));
    return QFileSystemModel::remove(aindex);
}

void FileSystemModel::update(const QString & path) const
{
//    qDebug() << "FileSystemModel::update" << path;
    QFileInfoGatherer & fileInfoGatherer = reinterpret_cast<QFileSystemModelPrivate*>(qGetPtrHelper(d_ptr))->fileInfoGatherer;
    QMetaObject::invokeMethod(&fileInfoGatherer, "list", Qt::DirectConnection, Q_ARG(QString, path));
    QMetaObject::invokeMethod(&fileInfoGatherer, "updateFile", Qt::DirectConnection, Q_ARG(QString, path));
}



