#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QtCore/qglobal.h>

#if defined(FILESYSTEMMODEL_LIBRARY)
#  define FILESYSTEMMODELSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FILESYSTEMMODELSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QFileSystemModel>

class FILESYSTEMMODELSHARED_EXPORT FileSystemModel : public QFileSystemModel
{
Q_OBJECT
public:
    explicit FileSystemModel(QObject *parent = 0);

    bool rmdir(const QModelIndex &aindex) const;
    bool remove(const QModelIndex &aindex) const;
    void update(const QString & path) const;

signals:

public slots:

};

#endif // FILESYSTEMMODEL_H
