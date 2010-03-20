#ifndef IEDITOR_H
#define IEDITOR_H

#include <QtCore/QString>
#include <QObject>

//#include "ieditorfactory.h"

class QToolBar;
class QWidget;
class IEditorFactory;
class IEditor
{
    IEditorFactory * m_factory;
public:
    IEditor() {}
    virtual ~IEditor() {}
    virtual bool open(const QString &filePath) = 0;
    virtual void close() {}
    virtual void save(const QString &/*filePath*/) {}
    virtual QWidget * widget() = 0;
    virtual QToolBar * toolBar () = 0;

    void setFactory(IEditorFactory* factory) { m_factory = factory; }
    const IEditorFactory * factory() const { return m_factory; }
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IEditor, "IEditor/1.0")
QT_END_NAMESPACE

#endif // IEDITOR_H
