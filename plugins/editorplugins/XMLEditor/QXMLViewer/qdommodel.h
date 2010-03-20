#ifndef QDOMMODEL_H
#define QDOMMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>

//class QDomModelPrivate;
class DomItem;
class QDomModel : public QAbstractItemModel
{
Q_OBJECT
//Q_DECLARE_PRIVATE(QDomModel)
public:
    explicit QDomModel(QObject *parent = 0);
    ~QDomModel();
    void setDomDocument(QDomDocument document);

    // repimplemented functions
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

signals:

public slots:

private:
    DomItem *rootItem;

    void clear();
    DomItem * item(const QModelIndex & index) const { return static_cast<DomItem*>(index.internalPointer()); }

protected:
//?    QDomModelPrivate * d_ptr;
};

#endif // QDOMMODEL_H
