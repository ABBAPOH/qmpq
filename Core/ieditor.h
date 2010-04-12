#ifndef IEDITOR_H
#define IEDITOR_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include "core.h"
//#include "ieditorfactory.h"

class QToolBar;
class QWidget;
class IEditor : public QObject
{
    Q_OBJECT
public:
    IEditor() {}
    virtual ~IEditor() {}
    virtual bool canHandle(const QString & /*filePath*/) { return false; }
    virtual bool canSave() { return false; }
    virtual void close() {}
    virtual QString currentFile() = 0;
    virtual bool isModified() { return false; }
    virtual bool open(const QString & /*filePath*/) = 0;
    virtual void save(const QString & /*filePath*/) {}
    virtual QToolBar * toolBar () = 0;
    virtual QWidget * widget() = 0;

signals:
    void changed();
};

//QT_BEGIN_NAMESPACE
//Q_DECLARE_INTERFACE(IEditor, "IEditor/1.0")
//QT_END_NAMESPACE

#endif // IEDITOR_H
