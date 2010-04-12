#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QtCore/QObject>

class FileManager : public QObject
{
Q_OBJECT
public:
    explicit FileManager(QObject *parent = 0);
    QStringList getOpenFileNames();
    QString getSaveFileName();
    int confirmClose(const QString & file);

signals:

public slots:

};

#endif // FILEMANAGER_H
