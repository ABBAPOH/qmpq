#include "qxmlviewer.h"

#include <QtGui/QResizeEvent>

#include "qdommodel.h"

QXMLViewer::QXMLViewer(QWidget *parent)
    : QWidget(parent),
     treeView(new QTreeView(this)),
     model(new QDomModel(this))
{
    treeView->setModel(model);
    setMinimumSize(300, 200);
//    connect(treeView, SIGNAL(expanded(const QModelIndex &)), SLOT(onExpand(const QModelIndex &)));
    treeView->setColumnWidth(0, 200);
    treeView->setColumnWidth(1, 200);
//    treeView->setColumnWidth(2, 100);
}

QXMLViewer::~QXMLViewer()
{

}

void QXMLViewer::setDocument(QDomDocument document)
{
    model->setDomDocument(document);
}

bool QXMLViewer::setContent(const QByteArray & data, bool namespaceProcessing)
{
    QDomDocument document;
    bool result = document.setContent(data, namespaceProcessing);
    setDocument(document);
    return result;
}

bool QXMLViewer::setContent(const QString & text, bool namespaceProcessing)
{
    QDomDocument document;
    bool result = document.setContent(text, namespaceProcessing);
    setDocument(document);
    return result;
}

bool QXMLViewer::setContent(QIODevice * dev, bool namespaceProcessing)
{
    qDebug("test");
    QDomDocument document;
    bool result = document.setContent(dev, namespaceProcessing);
    if(!result)
        qDebug("AAAA");
    setDocument(document);
    return result;
}

//void Widget::onExpand(const QModelIndex & index)
//{
//    for (int column = 0; column < model->columnCount(); column++) {
//        treeView->resizeColumnToContents(column);
//    }
//}

void QXMLViewer::resizeEvent(QResizeEvent *event)
{
    treeView->resize(event->size());
}
//void paintEvent(QPaintEvent * event) {}
