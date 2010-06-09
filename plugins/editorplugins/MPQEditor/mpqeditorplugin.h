#ifndef MPQEDITORPLUGIN_H
#define MPQEDITORPLUGIN_H

#include <ieditorfactory.h>
#include <iplugin.h>


namespace Core {

    const char * const ACTION_ADD = "ADD";
    const char * const ACTION_EXTRACT = "EXTRACT";
    const char * const ACTION_RENAME = "RENAME";
    const char * const ACTION_REMOVE = "REMOVE";

    const char * const ACTION_REOPEN = "REOPEN";
    const char * const ACTION_COMPACT = "COMPACT";
    const char * const ACTION_SET_HASH_TABLE_SIZE = "SET HASH TABLE SIZE";
    const char * const ACTION_CHANGE_ATTRIBUTES = "CHANGE ATTRIBUTES";
    const char * const ACTION_VERIFY_ARCHIVE = "VERIFY ARCHIVE";
    const char * const ACTION_VERIFY_FILES = "VERIFY FILES";

    const char * const ACTION_ARCHIVE_PROPERTIES = "ARCHIVE PROPERTIES";
    const char * const ACTION_FILE_PROPERTIES = "FILE PROPERTIES";
}

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
    virtual void close();
    virtual QString currentFile();
    virtual bool open(const QString &file);
    virtual QToolBar * toolBar ();
    virtual QWidget * widget();

private:
    void initActions();

    MPQEditor * m_editor;
    QToolBar * m_toolBar;
    QSignalMapper * viewModeMapper;
    QActionGroup * viewModeActionGroup;
    QAction * viewModeActions[5];
    QAction * actionOpen;
    QAction * actionOpenInNewTab;
    QAction * actionOpenInNewWindow;
    QAction * actionOpenExternally;
    QAction * actionNew_Folder;

public slots:
    void open();
    void openInNewTab();
    void openInNewWindow();
    void openExternally();

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

class QMPQArchive;
class QMPQArchiveEx;
class MPQEditorPlugin : public IPlugin
{
    Q_OBJECT
public:
    MPQEditorPlugin(QObject * parent = 0) : IPlugin(parent) {}
    ~MPQEditorPlugin() {}

    void initialize();
    void shutdown();
    QString name() { return "MPQ Editor plugin"; }
    QString description() { return QString(); }

    static bool canHandle(const QString &file);

private slots:
    void updateActions();
    void add();
    void extract();
    void rename();
    void remove();
    void reopen();
    void compact();
    void setHashTableSize();
    void changeAttributes();
    void verifyArchive();
    void verifyFiles();
    void archiveProperties();
    void fileProperties();

private:
    MPQEditorInterface * editor();
    MPQEditor * editorWidget();
    QMPQArchiveEx * getArchive(const QString & filePath);
    void initActions();

    QAction * actionReopen;
    QAction * actionSetHashTableSize;
    QAction * actionCompact;

    QAction * actionAdd;
    QAction * actionExtract;
    QAction * actionRename;
    QAction * actionRemove;

    QAction * actionChangeAttributes;

    QAction * actionVerifyArchive;
    QAction * actionVerifyFiles;

    QAction * actionArchiveProperties;
    QAction * actionFileProperties;
};

#endif // MPQEDITORPLUGIN_H
