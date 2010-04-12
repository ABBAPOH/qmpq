#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QString>

class IPlugin : public QObject
{
    Q_OBJECT

public:
    IPlugin(QObject * parent = 0) : QObject(parent) {}
    virtual ~IPlugin() {}

    virtual void initialize() = 0;
    virtual void shutdown() = 0;
    virtual QString name() = 0;
    virtual QString description() { return QString(); };
};

#endif // IPLUGIN_H
