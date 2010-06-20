#ifndef EDITORFACTORYMANAGER_H
#define EDITORFACTORYMANAGER_H

#include "ieditorfactorymanager.h"

class IEditorFactory;
class EditorFactoryManager : public IEditorFactoryManager
{
    Q_OBJECT
public:
    explicit EditorFactoryManager(QObject * parent = 0);

    void addFactory(IEditorFactory * factory);
    bool close(IEditor * editor);
    IEditor * create(const QString & path);
    virtual IEditor * currentEditor() { return m_editor; }
    virtual QIcon icon(const QString & path);
    void setCurrentEditor(IEditor * editor);

private:
    QList<IEditorFactory *> m_factories;
    IEditor * m_editor;
};

#endif // EDITORFACTORYMANAGER_H
