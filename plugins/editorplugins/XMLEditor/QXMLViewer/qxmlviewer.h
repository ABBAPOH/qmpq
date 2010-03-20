#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>

#include <QHBoxLayout>
#include <QTreeView>
#include <QFile>

#include <QDomDocument>

class QDomModel;
class QXMLViewer : public QWidget
{
    Q_OBJECT
public:
    QXMLViewer(QWidget *parent = 0);
    ~QXMLViewer();

public slots:
    void setDocument(QDomDocument document);
    bool setContent(const QByteArray & data, bool namespaceProcessing = false);
    bool setContent(const QString & text, bool namespaceProcessing = false);
    bool setContent(QIODevice * dev, bool namespaceProcessing = false);

private slots:
//    void onExpand(const QModelIndex &);

private:
//    QHBoxLayout * layout = new QHBoxLayout(this);
    QTreeView * treeView;
    QDomModel * model;
    QDomDocument document;

protected:
    void resizeEvent(QResizeEvent *event);
//    void paintEvent(QPaintEvent * event);

};

#endif // WIDGET_H
