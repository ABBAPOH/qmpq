#ifndef QMPQIMAGEVIEWER_H
#define QMPQIMAGEVIEWER_H

#include <QWidget>

namespace Ui
{
    class ImageViewer;
    class ImageSettingsDialog;
}
class ImageSettingsDialog;
class QLabel;

class ImageViewer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage);
    Q_PROPERTY(bool modified READ modified WRITE setModified);

public:
    ImageViewer(QWidget *parent = 0);
    ~ImageViewer();
    QImage image() { return m_image; }
    bool modified() { return m_modified; }

public slots:
    void setImage(const QImage & image);
//    void preferences();
    void clear();
    void copy();
    void paste();
    void zoomIn();
    void zoomOut();
    void zoomReset();
    void setModified(bool modified);

signals:
    void modificationChanged(bool changed);

private:
    Ui::ImageViewer *ui;
    QImage m_image;
    QLabel * label;
    double scale;
    bool m_modified;
};

#endif // QMPQIMAGEVIEWER_H
