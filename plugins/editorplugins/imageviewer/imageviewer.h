#ifndef QMPQIMAGEVIEWER_H
#define QMPQIMAGEVIEWER_H

#include <QWidget>
#include <QSettings>

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
public:
    ImageViewer(QWidget *parent = 0);
    ~ImageViewer();
    QImage image() { return m_image; }

public slots:
    void setImage(const QImage & image);
    void clear();
    void preferences();
    void copy();
    void paste();
    void zoomIn();
    void zoomOut();
    void zoomReset();

private:
    Ui::ImageViewer *ui;
    QImage m_image;
    QSettings * m_settings;
    QLabel * label;
    double scale;

    void savePreferences(const ImageSettingsDialog * dialog);
    void loadPreferences(ImageSettingsDialog * dialog);
};

#endif // QMPQIMAGEVIEWER_H
