#ifndef MPQEDITORPLUGIN_H
#define MPQEDITORPLUGIN_H

#include "../../../app/ieditorfactory.h"

class QXMLViewer;
class QSignalMapper;
class QAction;
class XMLEditorPlugin : public QObject, public IEditor
{
    Q_OBJECT
    Q_INTERFACES(IEditor)
public:
    XMLEditorPlugin(QXMLViewer * editor);
    ~XMLEditorPlugin();
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

#endif // MPQEDITORPLUGIN_H
