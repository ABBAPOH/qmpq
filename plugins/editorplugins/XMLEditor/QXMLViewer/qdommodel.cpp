#include "qdommodel.h"
//#include "qdommodel_p.h"

#include "domitem.h"

#include <QStringList>

QDomModel::QDomModel(QObject *parent) :
    QAbstractItemModel(parent),
    rootItem(0)
{
}

QDomModel::~QDomModel()
{
    clear();
}

void QDomModel::setDomDocument(QDomDocument document)
{
    clear();
    rootItem = new DomItem(document);
    reset();
}

int QDomModel::columnCount(const QModelIndex & parent) const
{
    return 3;
}

QVariant QDomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
         return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DomItem *domItem = static_cast<DomItem*>(index.internalPointer());

    QDomNode domNode = domItem->domNode();
    QStringList attributes;
    QDomNamedNodeMap nameNodeMap = domNode.attributes();

    switch (index.column()) {
         case 0:
             return domNode.nodeName();
         case 1:
             for (int i = 0; i < nameNodeMap.count(); ++i) {
                 QDomNode attribute = nameNodeMap.item(i);
                 attributes << attribute.nodeName() + "=\"" + attribute.nodeValue() + "\"";
             }
             return attributes.join(" ");
         case 2:
             if (domItem->hasChildren)
             return domNode.nodeValue().split("\n").join(" ");
             else
                 return domItem->m_domValueNode.nodeValue().split("\n").join(" ");
         default:
             return QVariant();
     }
}

Qt::ItemFlags QDomModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant QDomModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return tr("Name");
        case 1: return tr("Attributes");
        case 2: return tr("Value");
        default: return QVariant();
        }
    }

    return QVariant();
}

QModelIndex QDomModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = item(parent);

    DomItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex QDomModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = item(child);
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int QDomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0 || !rootItem)
        return 0;

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = item(parent);

    return parentItem->rows();
}

void QDomModel::clear()
{
    if (rootItem)
        delete rootItem;
    rootItem = 0;
}
