#include <QImage>
#include <QDebug>
#include <math.h>

#include <QLabel>

struct ColorData
{
    ColorData()
    {
    references = 0;
    red = 0;
    green = 0;
    blue = 0;
    }
    unsigned long references;
    unsigned long red;
    unsigned long green;
    unsigned long blue;

};

struct OcTreeNode
{
    OcTreeNode(OcTreeNode * parent, int pos = 0)
    {
        data = 0;
        if (parent) {
            this->parent = parent;
            parent->children[pos] = this;
        }
        for (int i = 0; i< 8; i++)
            children[i] = 0;
    }

    ColorData *data;
    OcTreeNode * parent;
    OcTreeNode * children[8];
    int pos;
};

int f(unsigned red, unsigned green, unsigned blue, int pos)
{
    return ((red >> (7 - pos) & 0x01) << 2) +
            ((green >> (7 - pos) & 0x01) << 1) +
            ((blue >> (7 - pos) & 0x01) << 0);
}

int refCount(OcTreeNode * n)
{
    int res = 0;
    for (int i = 0; i< 8; i++) {
        if (n->children[i] && n->children[i]->data)
            res += n->children[i]->data->references;
    }
    return res;
}

bool ocTreeLessThan(OcTreeNode * n1, OcTreeNode * n2)
{
    return refCount(n1) < refCount(n2);
}

int reduceNode(OcTreeNode * node)
{
    int childs = -1;
    for (int n = 0; n<8; n++)
    {
        if (node->children[n]) {
            childs++;
            if (!node->data)
                node->data = new ColorData;
            node->data->references += node->children[n]->data->references;
            node->data->red       += node->children[n]->data->red;
            node->data->green     += node->children[n]->data->green;
            node->data->blue      += node->children[n]->data->blue;
            // free the node. We don't need it anymore
            delete (node->children[n]);
            node->children[n] = 0;
        }
    }
//    qDebug() << "reducing" << node->pos << node->data->references <<
//            node->data->red <<
//            node->data->green <<
//            node->data->blue
//            << "clilds = " << childs;
    return childs;
}

void findLeaves(OcTreeNode * node, QList<OcTreeNode*> &result)
{
//    bool leaf = true;
    for (int i = 0; i< 8; i++){
        if (node->children[i]) {
//            leaf = false;
            findLeaves(node->children[i], result);
        }
    }
    if (node->data && !result.contains(node))
        result.append(node);
}

void createPalette(const QImage & image, QRgb * palette)
{
    OcTreeNode * rootNode = new OcTreeNode(0);
    OcTreeNode * currentNode = 0;
    int leafCount = 0;
    QList<OcTreeNode*> parents;

    qDebug() << image.width() << image.height();
    for (int i = 0; i < image.width(); i++)
        for (int j = 0; j < image.height(); j++) {
            QRgb pixel = image.pixel(i, j);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);
            currentNode = rootNode;
            for (int pos = 0; pos < 8; pos++) {

                int childNum = f(red, green, blue, pos);
                OcTreeNode * childNode = currentNode->children[childNum];
                if (!childNode) {
                    childNode = new OcTreeNode(currentNode, childNum);
                    childNode->pos = pos;
                    if (pos == 6) {
                        parents.append(childNode);
                    }
                }
                currentNode = childNode;

                if (pos == 7) {
                    if (!currentNode->data) {
                        currentNode->data = new ColorData;
                        leafCount++;
                    }
                    currentNode->data->red += red;
                    currentNode->data->green += green;
                    currentNode->data->blue += blue;
                    currentNode->data->references += 1;
                }
            }
        }
    qSort(parents.begin(), parents.end(), ocTreeLessThan);
    QList<OcTreeNode *> second;
//    qDebug() << "leaf count = " << leafCount;
    while (leafCount > 256) {
//        QList<OcTreeNode*> tests;

//        findLeaves(rootNode, tests);
//        qDebug() << "test" << tests.count();
//        qDebug() << "reducing" << leafCount << "leaves left";
        if (parents.isEmpty()) {
            parents.append(second);
            second.clear();
            qSort(parents.begin(), parents.end(), ocTreeLessThan);
        }
        OcTreeNode * node = parents.takeFirst();
        leafCount -= reduceNode(node);
        if (!second.contains(node->parent)) {
            second.append(node->parent);
//            leafCount ++;
//            qSort(parents.begin(), parents.end(), ocTreeLessThan);
        }
    }

    qDebug() << leafCount;

//    QRgb palette[256];
    QList<OcTreeNode*> leaves;

    findLeaves(rootNode, leaves);
    for (int index = 0; index < 256; index++) {
//        qDebug() << "loop" << index << leaves.count();
        OcTreeNode * currentnode = leaves.at(index >= leaves.count() ? leaves.count() -1 : index);
//        OcTreeNode * currentnode = leaves.at(index);
        int red   = currentnode->data->red / currentnode->data->references;
        int green = currentnode->data->green / currentnode->data->references;
        int blue  = currentnode->data->blue / currentnode->data->references;
//        qDebug() << currentnode->data->references << red << green << blue;
        palette[index] = qRgb(red, green, blue);
    }
    QImage img(8, 8, QImage::Format_RGB32);
    for(int i = 0; i< 8; i++)
        for(int j = 0; j< 8; j++) {
            img.setPixel(i,j, palette[i*8 + j]);
    }
//    QLabel * l = new QLabel;
//    l->setPixmap(QPixmap().fromImage(img.scaled(32, 32)));
//    l->show();
    qDebug() << "written";
}

double calcColorDistance(QRgb color1, QRgb color2)
{
    int dRed = qRed(color1) - qRed(color2);
    int dGreen = qGreen(color1) - qGreen(color2);
    int dBlue = qBlue(color1) - qBlue(color2);
    return sqrt(dRed*dRed + dGreen*dGreen + dBlue*dBlue);
}

quint8 getNearestColor(QRgb color, QRgb *palette)
{
    quint8 pos = 0;
    double dist = calcColorDistance(color, palette[0]);
    for (int i = 0; i < 256; i++) {
        double currentDist = calcColorDistance(color, palette[i]);
        if (dist > currentDist) {
            pos = i;
            dist = currentDist;
        }
    }
    return pos;
}

