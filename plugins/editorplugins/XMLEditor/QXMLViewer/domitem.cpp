#include "domitem.h"

#include <QDebug>

DomItem::DomItem(QDomNode & domNode, DomItem * parent, int row) :
        m_parent(parent),
        m_isPopulated(false),
        m_domNode(domNode),
        m_row(row),
        hasChildren(true)
{
}

DomItem::~DomItem()
{
    foreach(DomItem * item, m_children) {
        delete item;
    }
}

int DomItem::rows()
{
    if (!m_isPopulated)
        populate();

    return m_children.count();
}

DomItem * DomItem::child(int at)
{
    if (!m_isPopulated)
        populate();

    if (at >= 0 && at < m_children.count())
        return m_children.at(at);
    else
        return 0;
}

void DomItem::populate()
{
    int row = 0;
    m_isPopulated = true;
    const QDomNodeList & nodeList = m_domNode.childNodes();
    if (nodeList.count() == 1) {
        QDomNode node = nodeList.at(0);
        if (!node.hasChildNodes()) {
            m_domValueNode = node;
            //        m_domNode = nodeList.at(0);
            hasChildren = false;
            return;
        }
    }
    qDebug() << nodeList.count();
    for (row = 0; row < nodeList.count(); row++) {
        QDomNode node = nodeList.item(row);
        DomItem * item = new DomItem(node, this, row);
        m_children.append(item);
    }
}
