#ifndef MPQEDITORPLUGIN_H
#define MPQEDITORPLUGIN_H

#include <ieditorfactory.h>
#include <iplugin.h>

class CodeEditor;
class QSignalMapper;
class QAction;
class CodeEditorInterface :/* public QObject,*/ public IEditor
{
    Q_OBJECT
//    Q_INTERFACES(IEditor)
public:
    CodeEditorInterface(CodeEditor * editor);
    ~CodeEditorInterface();
    virtual bool canSave() { return true; }
    virtual QString currentFile() { return m_currentFile; }
    virtual bool isModified();
    virtual bool open(const QString &filePath);
    virtual void save(const QString &filePath);
    virtual QWidget * widget();
    virtual QToolBar * toolBar ();
    virtual void close();

private:
    void initToolBar();

    CodeEditor * m_editor;
    QString m_currentFile;
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

class CodeEditorPlugin : public IPlugin
{
    Q_OBJECT

public:
    CodeEditorPlugin(QObject * parent = 0) : IPlugin(parent) {}
    ~CodeEditorPlugin() {}

    void initialize();
    void shutdown() {};
    QString name() { return "Code Editor plugin"; };
    QString description() { return QString(); };
};

#endif // MPQEDITORPLUGIN_H
