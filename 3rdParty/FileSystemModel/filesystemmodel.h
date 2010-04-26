#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>

class FileSystemModel : public QFileSystemModel
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
