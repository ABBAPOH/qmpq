#ifndef MPQEDITORPLUGIN_H
#define MPQEDITORPLUGIN_H

#include <ieditorfactory.h>
#include <iplugin.h>

class MPQEditor;
class QSignalMapper;
class QAction;
class QActionGroup;
class QContextMenuEvent;
class MPQEditorInterface : /*public QObject,*/ public IEditor
{
    Q_OBJECT
//    Q_INTERFACES(IEditor)
public:
    MPQEditorInterface(MPQEditor * editor);
    ~MPQEditorInterface();
    virtual bool canHandle(const QString & filePath);
    virtual QString currentFile();
    virtual bool open(const QString &file);
    virtual QWidget * widget();
    virtual QToolBar * toolBar ();
    virtual void close();

private:
    void initActions();

    MPQEditor * m_editor;
    QToolBar * m_toolBar;
    QSignalMapper * viewModeMapper;
    QActionGroup * viewModeActionGroup;
    QAction * viewModeActions[5];
    QAction * actionAdd;
    QAction * actionExtract;
    QAction * actionRename;
    QAction * actionRemove;
    QAction * actionNew_Folder;

public slots:
    void add();
    void extract();
    void setViewMode(int mode);
    void openRequest(const QString &file);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    bool contextMenu(QContextMenuEvent *event);
};

class MPQEditorFactory : public QObject, public IEditorFactory
{
    Q_OBJECT
    Q_INTERFACES(IEditorFactory)
public:
    virtual QString name() { return "MPQEditor"; }
//    IEditor * instance(QWidget * parent);
    IEditor * createEditor(QWidget * parent);
    void shutdown();
    bool canHandle(const QString &file) const;

    MPQEditorFactory();
};

class MPQEditorPlugin : public IPlugin
{
    Q_OBJECT
public:
    MPQEditorPlugin(QObject * parent = 0) : IPlugin(parent) {}
    ~MPQEditorPlugin() {}

    void initialize();
    void shutdown();
    QString name() { return "MPQ Editor plugin"; };
    QString description() { return QString(); };

    static bool canHandle(const QString &file);
};

#endif // MPQEDITORPLUGIN_H
