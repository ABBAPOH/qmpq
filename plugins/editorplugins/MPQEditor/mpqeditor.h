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
class MPQEditor : public QWidget
{
    Q_OBJECT
public:
    explicit MPQEditor(QWidget *parent = 0);
    enum ViewMode { ListView = 0, IconView, TableView, ColumnView, TreeView };

    bool canUp();
    static QAbstractItemModel * model();
    void showColumns(bool show);

private:
//    const int test = 5;
    QString m_currentFile;
    QListView * listView;
    QColumnView * columnView;
    QTableView * tableView;
    QTreeView * treeView;
    QAbstractItemView * views[MaxViews];
    QAbstractItemView * currentView;
    ViewMode m_viewMode;
    static QDirModel * m_model;
    QModelIndexList selectedIndexes();

signals:
    void openRequested(const QString &file);
    void currentChanged(const QString &file);

public slots:
    void open(const QString &file);
    void closeFile();
    void add(const QStringList & files);
    void extract(const QString & destDir);
    void remove();
    void setViewMode(ViewMode mode);
    void up();
//    void remove();
private slots:
    void onDoubleClick(const QModelIndex & index);
};

#endif // MPQEDITOR_H
