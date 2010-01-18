#include "treeitem.h"

#include <QStringList>
#include <QDebug>

TreeItem::TreeItem(TreeItem *parent, bool isDir)
{
    parentItem = parent;
    m_dir = isDir;
    m_unknown = false;
//    name = "root";
//    m_fileData.dwCompressedSize = 0;
//    m_fileData.dwFullSize = 0;
//    itemData = data;
}

TreeItem::~TreeItem()
{
//    qDebug() << "TreeItem::~TreeItem";
    qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem *child)
{
//    m_dir = true;
    childItems.append(child);
}

/*void TreeItem::appendChild(QStringList & path, FILELISTENTRY & fileData)
{
    qDebug() << "appendChild" << path;

    if (path.count() == 1) {
        TreeItem * item = new TreeItem(this);
//        item->name = path.first();
        item->setData(Name, path.first());
        //item->m_fileData = fileData;
        childItems.append(item);
    } else if (path.count() >= 1) {
        QString name = path.takeFirst();
        TreeItem * item = 0;
        foreach (TreeItem * child, childItems) {
//            if (child->name == name) {
            if (child->data(Name) == name) {
                item = child;
            }
        }
        if (!item) {
            item = new TreeItem(this);
            item->setData(Name, name);
//            item->name = name;
//            item->m_fileData = fileData;
            childItems.append(item);
        }

        item->appendChild(path, fileData);
    }

//    childItems.append(item);
}*/

void TreeItem::deleteChild(TreeItem * child)
{
    childItems.removeAll(child);
    delete child;
}

TreeItem *TreeItem::child(int row)
{
    return childItems.at(row);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::columnCount() const
{
//    return itemData.count();
    return m_columns;
}

QVariant TreeItem::data(int column) const
{
//    return itemData.value(column);
    return m_data[column];
}

void TreeItem::setData(int column, const QVariant & data)
{
    m_data[column] = data;
}

TreeItem *TreeItem::parent()
{
    return parentItem;
}

int TreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

static int savedColumn = 0;
bool lessThan1(const TreeItem * item1, const TreeItem * item2)
{
    return item1->data(savedColumn).toString() < item2->data(savedColumn).toString();
}

bool greaterThan(const TreeItem * item1, const TreeItem * item2)
{
    return item1->data(savedColumn).toString() > item2->data(savedColumn).toString();
}

void TreeItem::sort(int column, Qt::SortOrder order)
{
    savedColumn = column;
    if (order == Qt::AscendingOrder)
        qSort(childItems.begin(), childItems.end(), lessThan1);
    else
        qSort(childItems.begin(), childItems.end(), greaterThan);
    foreach(TreeItem * item, childItems) {
        item->sort(column, order);
    }
}
