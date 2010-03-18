#ifndef MPQEDITORPLUGIN_H
#define MPQEDITORPLUGIN_H

#include "../../../app/ieditorfactory.h"

class CodeEditor;
class QSignalMapper;
class QAction;
class CodeEditorPlugin : public QObject, public IEditor
{
    Q_OBJECT
    Q_INTERFACES(IEditor)
public:
    CodeEditorPlugin(CodeEditor * editor);
    ~CodeEditorPlugin();
    virtual bool open(const QString &file);
    virtual QWidget * widget();
    virtual QToolBar * toolBar ();
    virtual void close();

private:
    void initToolBar();

    CodeEditor * m_editor;
    QToolBar * m_toolBar;
    QAction * viewModeActions[5];

public slots:
};

class CodeEditorFactory : public QObject, public IEditorFactory
{
    Q_OBJECT
    Q_INTERFACES(IEditorFactory)
public:
    IEditor * createEditor(QWidget * parent);
    void shutdown();
    bool canHandle(const QString &file) const;

    CodeEditorFactory();
};

#endif // MPQEDITORPLUGIN_H
