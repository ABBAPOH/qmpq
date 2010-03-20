#ifndef IEDITORFACTORY_H
#define IEDITORFACTORY_H

#include <QtCore/QStringList>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include "ieditor.h"

//class IEditor;
class IEditorFactory
{
public:
    IEditorFactory() {}
    virtual ~IEditorFactory() {}
    virtual QString name() { return ""; }
    IEditor * instance(QWidget * parent = 0)
    {
        IEditor * result = createEditor(parent);
        result->setFactory(this);
        return result;
    }
    virtual void shutdown() {}

    virtual bool canHandle(const QString &file) const = 0;
    virtual QString saveFilter() const { return ""; }

private:
    virtual IEditor * createEditor(QWidget * parent = 0) = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IEditorFactory, "IEditorFactory/1.0")
QT_END_NAMESPACE

#endif // IEDITORFACTORY_H
