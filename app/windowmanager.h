#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <iwindowmanager.h>

class WindowManager : public IWindowManager
{
    Q_OBJECT

public:
    explicit WindowManager(QObject *parent = 0);
    bool open(const QString path);
    bool openInNewTab(const QString path);
    bool openInNewWindow(const QString path) {};
    bool openExternally(const QString path);
    bool save(const QString path = "");
    bool close();

signals:

public slots:

};

#endif // WINDOWMANAGER_H
