#ifndef QDOMMODEL_P_H
#define QDOMMODEL_P_H

#include <QDomDocument>
#include "domitem.h"

class QDomModelPrivate
{
private:
    QDomDocument domDocument;
    DomItem *rootItem;
};

#endif // QDOMMODEL_P_H
