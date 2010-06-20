#ifndef IEDITORSFACTORY_H
#define IEDITORSFACTORY_H

#include <QtCore/QObject>
#include <QtGui/QIcon>

class IEditor;
class IEditorFactory;
class IEditorFactoryManager : public QObject
{
    Q_OBJECT
public:
    IEditorFactoryManager(QObject * parent = 0) : QObject(parent) {}
    virtual ~IEditorFactoryManager() {}

    virtual void addFactory(IEditorFactory * factory) = 0;
    virtual bool close(IEditor * editor) = 0;
    virtual IEditor * create(const QString & path) = 0;
    virtual IEditor * currentEditor() = 0;
    virtual QIcon icon(const QString & path) = 0;
    virtual void setCurrentEditor(IEditor * editor) = 0;

signals:
    void currentEditorChanged(IEditor *);
};

#endif // IEDITORSFACTORY_H
