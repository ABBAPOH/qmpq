#ifndef IWINDOWMANAGER_H
#define IWINDOWMANAGER_H

#include <QtCore/QObject>

class IWindowManager : public QObject
{
    Q_OBJECT
public:
    IWindowManager(QObject * parent = 0) : QObject(parent) {}
    virtual ~IWindowManager() {}

    virtual bool open(const QString path) = 0;
    virtual bool openInNewTab(const QString path) = 0;
    virtual bool openInNewWindow(const QString path) = 0;
    virtual bool save(const QString path = "") = 0;
    virtual bool close() = 0;
};

#endif // IWINDOWMANAGER_H
