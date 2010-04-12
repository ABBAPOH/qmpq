#include "basecontext.h"

BaseContext::BaseContext(QWidget * widget, QMainWindow * mainWindow)
{
    m_widget = widget;
    m_mainWindow = mainWindow;
}

QWidget * BaseContext::widget()
{
    return m_widget;
}

QMainWindow * BaseContext::mainWindow()
{
    return m_mainWindow;
}
