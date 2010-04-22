#ifndef MPQEDITOR_H
#define MPQEDITOR_H

#include <QtGui/QWidget>
#include <QtCore/QModelIndex>

#define MaxViews 5

class QAbstractItemView;
class QListView;
class QColumnView;
class QTableView;
class QTreeView;
class QDirModel;
class QAbstractItemModel;
class QStackedLayout;
class MPQEditor : public QWidget
{
    Q_OBJECT
public:
    explicit MPQEditor(QWidget *parent = 0);
    ~MPQEditor();
    enum ViewMode { ListView = 0, IconView, TableView, ColumnView, TreeView };

    bool canUp();
    QString currentFile() { return m_currentFile; }
    static QAbstractItemModel * model();
    void showColumns(bool show);

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
    static QDirModel * m_model;
    QAction * openAction;

    void initModel();
    void initViews();
    QModelIndexList selectedIndexes();
    QString selectedDir();
    void extract(const QString & path, const QString & destDir);
    void remove(const QModelIndex & path);

signals:
    void openRequested(const QString &file);
    void currentChanged(const QString &file);

public slots:
    void open(const QString &file);
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
