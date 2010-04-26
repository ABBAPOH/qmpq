#ifndef MPQEDITOR_H
#define MPQEDITOR_H

#include <QtGui/QWidget>
#include <QtCore/QModelIndex>

#define MaxViews 5

class MPQEditorError
{
public:
    QString errorString() const { return m_errorString; }
    void addSubError(const MPQEditorError &error) { m_suberrors.append( error); }
    QList<MPQEditorError> subErrors() const { return m_suberrors; }

private:
    QString m_errorString;
    QList<MPQEditorError> m_suberrors;
};

class QAbstractItemView;
class QListView;
class QColumnView;
class QTableView;
class QTreeView;
class QDirModel;
class FileSystemModel;
class QAbstractItemModel;
class QStackedLayout;
class QMPQFileEngine;
class MPQEditor : public QWidget
{
    Q_OBJECT
    Q_ENUMS(ViewMode)
public:
    explicit MPQEditor(QWidget *parent = 0);
    ~MPQEditor();
    enum ViewMode { ListView = 0, IconView, TableView, ColumnView, TreeView };

    bool canUp();
    QString currentFile() { return m_currentFile; }
    static QAbstractItemModel * model();
    QStringList selectedPaths();
    void showColumns(bool show);
    bool isMPQArchive();


private:
//    const int test = 5;
    QString m_currentFile;
    QListView * listView;
    QListView * iconView;
    QColumnView * columnView;
    QTableView * tableView;
    QTreeView * treeView;
    QAbstractItemView * views[MaxViews];
    QAbstractItemView * currentView;
    ViewMode m_viewMode;
    QStackedLayout * layout;
//    static QDirModel * m_model;
    static FileSystemModel * m_model;
    QAction * openAction;

    void initModel();
    void initViews();
    void initActions();
    QMPQFileEngine * getIndexMPQEngine(const QModelIndex & index);
    QModelIndexList selectedIndexes();
    QString selectedDir();
    void extract(const QString & path, const QString & destDir);
    void remove(const QModelIndex & path);

signals:
    void openRequested(const QString &file);
    void currentChanged(const QString &file);

public slots:
    void open(const QString &file);
    void reopenUsingListfile(const QByteArray &listfile);
    void closeFile();
    void add(const QStringList & files);
    void extract(const QString & destDir);
    void remove();
    void rename();
    void setViewMode(ViewMode mode);
    void up();
    void newFolder(const QString & name = "");
//    void remove();
private slots:
    void onDoubleClick(const QModelIndex & index);
    void onOpenRequest();
};

#endif // MPQEDITOR_H
