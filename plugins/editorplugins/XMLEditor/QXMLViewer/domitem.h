#ifndef DOMITEM_H
#define DOMITEM_H

#include <QList>
#include <QDomNode>

class DomItem
{
public:
    DomItem(QDomNode & domNode, DomItem * parent = 0, int row = 0);
    ~DomItem();
    DomItem * child(int at);
    DomItem * parent() { return m_parent; }
    QDomNode domNode() { return m_domNode; }
    int row() { return m_row; }
    int rows();
    bool hasChildren;
    QDomNode m_domValueNode;

private:
    DomItem * m_parent;
    QList <DomItem*> m_children;
    bool m_isPopulated;
    QDomNode m_domNode;
    int m_row;

    void populate();
};

#endif // DOMITEM_H
