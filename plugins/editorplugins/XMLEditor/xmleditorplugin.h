#ifndef MPQEDITORPLUGIN_H
#define MPQEDITORPLUGIN_H

#include <ieditorfactory.h>
#include <iplugin.h>

class QXMLViewer;
class QSignalMapper;
class QAction;
class XMLEditorInterface : public IEditor
{
    Q_OBJECT
public:
    XMLEditorInterface(QXMLViewer * editor);
    ~XMLEditorInterface();
    virtual QString currentFile() { return m_currentFile; }
    virtual bool open(const QString &file);
//    virtual void save(const QString &file);
    virtual void close();
    virtual QWidget * widget();
    virtual QToolBar * toolBar ();

private:
    void initToolBar();

    QXMLViewer * m_editor;
    QString m_currentFile;
    QToolBar * m_toolBar;
    QAction * viewModeActions[5];

public slots:
};

class XMLEditorFactory : public QObject, public IEditorFactory
{
    Q_OBJECT
    Q_INTERFACES(IEditorFactory)
public:
    IEditor * createEditor(QWidget * parent);
    void shutdown();
    bool canHandle(const QString &file) const;

    XMLEditorFactory();
};

class XMLEditorPlugin : public IPlugin
{
    Q_OBJECT

public:
    XMLEditorPlugin(QObject * parent = 0) : IPlugin(parent) {}
    virtual ~XMLEditorPlugin() {}

    virtual void initialize();
    virtual void shutdown() {};
    virtual QString name() { return "XML Editor Plugin"; }
    virtual QString description() { return QString(); };

};

#endif // MPQEDITORPLUGIN_H
