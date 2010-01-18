#ifndef QMPQIMAGEVIEWER_H
#define QMPQIMAGEVIEWER_H

#include <QMainWindow>
#include <QSettings>

namespace Ui
{
    class ImageViewer;
    class ImageSettingsDialog;
}
class ImageSettingsDialog;
class QLabel;

class ImageViewer : public QMainWindow
{
    Q_OBJECT
public:
    ImageViewer(QWidget *parent = 0);
    ~ImageViewer();
public slots:
    void open(const QString & path = QString());
    void closeFile();
//    void open();
    void save_As();
    void preferences();
    void copy();
signals:
    void fileSaved(const QString &);
private:
    Ui::ImageViewer *ui;
    QString currentFile;
    QImage m_currentImage;
    QSettings * m_settings;
    QLabel * label;

    void savePreferences(const ImageSettingsDialog * dialog);
    void loadPreferences(ImageSettingsDialog * dialog);
};

#endif // QMPQIMAGEVIEWER_H
