#ifndef MPQEDITORPLUGIN_H
#define MPQEDITORPLUGIN_H

#include "../../../app/ieditorfactory.h"

class MPQEditor;
class QSignalMapper;
class QAction;
class QActionGroup;
class QContextMenuEvent;
class MPQEditorPlugin : public QObject, public IEditor
{
    Q_OBJECT
    Q_INTERFACES(IEditor)
public:
    MPQEditorPlugin(MPQEditor * editor);
    ~MPQEditorPlugin();
    virtual bool open(const QString &file);
    virtual QWidget * widget();
    virtual QToolBar * toolBar ();
    virtual void close();
private:
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

#endif // MPQEDITORPLUGIN_H
