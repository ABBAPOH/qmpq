#ifndef UNIVERSALVIEW_H
#define UNIVERSALVIEW_H

#include <QtGui/QWidget>
#include <QtGui/QAbstractItemView>

class QAbstractItemView;
class QListView;
class QColumnView;
class QTableView;
class QTreeView;
class QDirModel;
class QAbstractItemModel;
class QStackedLayout; // implementation detail
class QItemSelectionModel;
class UniversalView : public QWidget
{
Q_OBJECT
Q_ENUMS(ViewMode)
#define MaxViews 5
public:
    explicit UniversalView(QWidget *parent = 0);
    ~UniversalView();
    enum ViewMode { ListView = 0, IconView, TableView, ColumnView, TreeView };
    void setModel(QAbstractItemModel * model);

    void setEditTriggers(QAbstractItemView::EditTriggers triggers);
    QAbstractItemView::EditTriggers editTriggers() const;

    // drag and drop
    bool acceptDrops() const;
    void setAcceptDrops(bool on);

    void edit(const QModelIndex & index);

    void setColumnWidth(int column, int width);

    void setDefaultDropAction(Qt::DropAction dropAction);
    Qt::DropAction defaultDropAction() const;

    void setDragDropOverwriteMode(bool overwrite);
    bool dragDropOverwriteMode() const;

    void setDragDropMode(QAbstractItemView::DragDropMode behavior);
    QAbstractItemView::DragDropMode dragDropMode() const;

    void setRootIndex(const QModelIndex & index);
    QModelIndex rootIndex() const { return m_rootIndex; }

    void setSelectionMode(QAbstractItemView::SelectionMode mode);
    QAbstractItemView::SelectionMode selectionMode() const;

	QItemSelectionModel * selectionModel() const;

    ViewMode viewMode() { return m_viewMode; }

signals:
    void doubleClicked(const QModelIndex &);

public slots:
    void setViewMode(ViewMode mode);

private:
    QListView * listView;
    QListView * iconView;
    QColumnView * columnView;
//    QTableView * tableView;
    QTreeView * tableView;
    QTreeView * treeView;
    QAbstractItemView * views[MaxViews];
    QAbstractItemView * m_currentView;
    ViewMode m_viewMode;
    QStackedLayout * layout;
//    static QDirModel * m_model;
    QAbstractItemModel * m_model;

    QModelIndex m_rootIndex;

    void initViews();
};

#endif // UNIVERSALVIEW_H
