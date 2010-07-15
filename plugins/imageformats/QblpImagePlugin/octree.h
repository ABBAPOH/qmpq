#ifndef OCTREE_H
#define OCTREE_H

#include <QImage>

class OcTreeNode;
class OcTree
{
public:
    OcTree(const QImage & image);
    ~OcTree();
    QRgb * palette();
    int index(QRgb pixel);

private:
    OcTreeNode * rootNode;
    QImage m_image;
    QRgb * m_palette;

    void createPalette();
};

#endif // OCTREE_H
