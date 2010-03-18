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
public:
    ImageViewer(QWidget *parent = 0);
    ~ImageViewer();
    QString currentFile() { return m_currentFile; }

public slots:
    void open(const QString & path = QString());
    void closeFile();
//    void open();
    void save(QString path = "");
    void preferences();
    void copy();
    void paste();
    void zoomIn();
    void zoomOut();
    void zoomReset();


signals:
    void fileSaved(const QString &);

private:
    Ui::ImageViewer *ui;
    QString m_currentFile;
    QImage m_currentImage;
    QSettings * m_settings;
    QLabel * label;
    double scale;

    void setImage(const QImage & image);
    void savePreferences(const ImageSettingsDialog * dialog);
    void loadPreferences(ImageSettingsDialog * dialog);
};

#endif // QMPQIMAGEVIEWER_H
