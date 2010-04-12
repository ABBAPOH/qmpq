#ifndef ICONTEXT_H
#define ICONTEXT_H

#include <QObject>

class QMainWindow;
class QWidget;
class IContext
{
public:
    IContext() {}
    virtual ~IContext() {}

    virtual QWidget * widget() = 0;
    virtual QMainWindow * mainWindow() = 0;
};

//QT_BEGIN_NAMESPACE
//Q_DECLARE_INTERFACE(IContext, "IContext/1.0")
//QT_END_NAMESPACE

#endif // ICONTEXT_H
