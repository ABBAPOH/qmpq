#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

class FileManager : public QObject
{
Q_OBJECT
public:
    explicit FileManager(QObject *parent = 0);
    QStringList getOpenFileNames();
    QString getSaveFileName();
    int confirmClose(const QString & file);
    void registerExtensionString(const QString & extensionString);

signals:

public slots:

private:
    QStringList m_registeredExtensionStrings;
    QString m_extensionString;
};

#endif // FILEMANAGER_H
