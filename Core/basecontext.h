#ifndef BASECONTEXT_H
#define BASECONTEXT_H

#include "icontext.h"

class BaseContext : public IContext
{
public:
    explicit BaseContext(QWidget * widget = 0, QMainWindow * mainWindow = 0);
    QWidget * widget();
    QMainWindow * mainWindow();

private:
    QWidget * m_widget;
    QMainWindow * m_mainWindow;
};

#endif // BASECONTEXT_H
