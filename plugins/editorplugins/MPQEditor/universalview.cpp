#include "universalview.h"

#include <QtGui/QListView>
#include <QtGui/QColumnView>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>
#include <QtGui/QStackedLayout>

UniversalView::UniversalView(QWidget *parent) :
    QWidget(parent),
    listView(new QListView(this)),
    iconView(new QListView(this)),
    columnView(new QColumnView(this)),
//    tableView(new QTableView(this)),
    tableView(new QTreeView(this)),
    treeView(new QTreeView(this)),
    m_currentView(0),
    layout(new QStackedLayout(this)),
    m_model(0)
{
    initViews();
    setViewMode(ListView);
    setLayout(layout);
    listView->setDragEnabled(true);
    listView->setDropIndicatorShown(true);

}

UniversalView::~UniversalView()
{

}

void UniversalView::edit(const QModelIndex & index)
{
    if (m_currentView)
        m_currentView->edit(index);
}

void UniversalView::setAcceptDrops(bool on)
{
//    this->setAcceptDrops(true);
    for (int i = 0; i < MaxViews; i++) {
        views[i]->setAcceptDrops(on);
    }
}

void UniversalView::setColumnWidth(int column, int width)
{
    tableView->setColumnWidth(column, width);
    treeView->setColumnWidth(column, width);
}

void UniversalView::setEditTriggers(QAbstractItemView::EditTriggers triggers)
{
    for (int i = 0; i < MaxViews; i++) {
        views[i]->setEditTriggers(triggers);
    }
}

void UniversalView::setDefaultDropAction(Qt::DropAction dropAction)
{
    for (int i = 0; i < MaxViews; i++) {
        views[i]->setDefaultDropAction(dropAction);
    }
}

void UniversalView::setDragDropMode(QAbstractItemView::DragDropMode behavior)
{
    for (int i = 0; i < MaxViews; i++) {
        views[i]->setDragDropMode(behavior);
    }
}

void UniversalView::setModel(QAbstractItemModel * model)
{
    m_model = model;
    for (int i = 0; i < MaxViews; i++) {
        views[i]->setModel(model);
    }
}

void UniversalView::setRootIndex(const QModelIndex & index)
{
    m_rootIndex = index;
    if (m_currentView)
        m_currentView->setRootIndex(index);
}

void UniversalView::setDragDropOverwriteMode(bool overwrite)
{
    for (int i = 0; i < MaxViews; i++) {
        views[i]->setDragDropOverwriteMode(overwrite);
    }
}

void UniversalView::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    for (int i = 0; i < MaxViews; i++) {
        views[i]->setSelectionMode(mode);
    }
}

QItemSelectionModel * UniversalView::selectionModel() const
{
	return m_currentView ? m_currentView->selectionModel() : 0;
}

void UniversalView::setViewMode(ViewMode mode)
{
    layout->setCurrentIndex(mode);

    views[mode]->setRootIndex(m_rootIndex);

//    if (mode < 3)
//        views[mode]->setRootIndex(m_currentView ? m_currentView->rootIndex() : QModelIndex() ); //sets the same directory for list and table views
//    else
//        views[mode]->setRootIndex(m_model->index(m_currentFile));

    m_viewMode = mode;
    m_currentView = views[mode];
}

void UniversalView::initViews()
{
    views[ListView] = listView;
    views[IconView] = iconView;
    views[TableView] = tableView;
    views[ColumnView] = columnView;
    views[TreeView] = treeView;

    for (int i = 0; i < MaxViews; i++)
        layout->addWidget(views[i]);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    iconView->setGridSize(QSize(150, 100));
    iconView->setWordWrap(true);
    iconView->setViewMode(QListView::IconMode);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setRootIsDecorated(false);

    for (int i = 0; i < MaxViews; i++) {
//        views[i]->setEditTriggers(QAbstractItemView::SelectedClicked);
//        views[i]->setDragDropMode(QAbstractItemView::DragDrop);
//        views[i]->setDragDropMode(QAbstractItemView::DropOnly);
//        views[i]->setDragEnabled(true);
//        views[i]->setAcceptDrops(true);
//        views[i]->setDragDropOverwriteMode(false);
//        views[i]->setDefaultDropAction(Qt::MoveAction);
//        views[i]->setSelectionMode(QAbstractItemView::ExtendedSelection);
        views[i]->setFocusPolicy(Qt::NoFocus);
        connect(views[i], SIGNAL(doubleClicked(const QModelIndex &)), SIGNAL(doubleClicked(const QModelIndex &)));
    }
}
