#ifndef TREEITEM_H
#define TREEITEM_H

#include <QtGui/QApplication>
#include <QVariant>

class TreeItem
{
    static const int m_columns = 4;
public:
//    TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
    TreeItem(TreeItem *parent = 0, bool isDir = false);
    virtual ~TreeItem();

    void appendChild(TreeItem *child);
    //void appendChild(QStringList & path, FILELISTENTRY & fileData);
    void deleteChild(TreeItem * child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setData(int column, const QVariant & data);
    int row() const;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    TreeItem *parent();
    void setData(int column);
    bool isDir() { return m_dir; /*return childItems.count() > 0;*/ }
    bool isFile() { return !m_dir; }
    #warning warning:)
//  TODO: find another decision
    bool inUnknown() { return m_unknown; }
    void setUnknown(bool unknown) { m_unknown = unknown; }
    bool remove(TreeItem * item) { return childItems.removeOne(item); }

//private:
    QList<TreeItem*> childItems;
    QVariant m_data[m_columns];
    TreeItem *parentItem;
    bool m_dir;
    bool m_unknown;
};


#endif
