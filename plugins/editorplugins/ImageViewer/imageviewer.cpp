#include "imageviewer.h"
#include "ui_imageviewer.h"
#include "ui_imagesettingsdialog.h"

#include <QImage>
#include <QImageWriter>
#include <QFileDialog>
#include <QClipboard>

#include <QDebug>

class ImageSettingsDialog: public Ui::ImageSettingsDialog, public QDialog
{
    public: ImageSettingsDialog(QWidget *parent = 0) : QDialog(parent) { setupUi(this); }
};

ImageViewer::ImageViewer(QWidget *parent)
        :QWidget(parent),
        ui(new Ui::ImageViewer),
        m_settings(new QSettings("QMPQ", "ImageViewer")),
        scale(1.0)
{
    ui->setupUi(this);
//    setWindowTitle(tr("QMPQ Image Viewer"));
//    ui->label->setScaledContents(true);
    label = new QLabel();
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->scrollArea->setWidget(label);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);

//    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(open()));
//    connect(ui->actionSave_As, SIGNAL(triggered()), SLOT(save_As()));
//    connect(ui->actionPreferences, SIGNAL(triggered()), SLOT(preferences()));
//    connect(ui->actionCopy, SIGNAL(triggered()), SLOT(copy()));

    if (m_settings->allKeys().isEmpty()) {
        ImageSettingsDialog dialog(this);
        savePreferences(&dialog);
    }
}

ImageViewer::~ImageViewer()
{
}

void ImageViewer::setImage(const QImage & image)
{
    m_image = image;

    if (!image.isNull())
        label->setPixmap(QPixmap().fromImage(m_image));
}

void ImageViewer::clear()
{
    qDebug("ImageViewer::clear");
    setImage(QImage());
}

//void ImageViewer::preferences()
//{
//    ImageSettingsDialog dialog(this);
//    loadPreferences(&dialog);
//    dialog.exec();
//    savePreferences(&dialog);
//}

void ImageViewer::copy()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(m_image);
}

void ImageViewer::paste()
{
    QClipboard *clipboard = QApplication::clipboard();
    setImage(clipboard->image());
    setModified(true);
}

void ImageViewer::zoomIn()
{
    scale += 0.1;
    int w = m_image.width(), h = m_image.height();
    QImage scaledImage = m_image.scaled(w*scale, h*scale);
    label->setPixmap(QPixmap().fromImage(scaledImage));
}

void ImageViewer::zoomOut()
{
    scale -= 0.1;
    if (scale <= 0.1) {
        scale = 0.1;
        return;
    }
    int w = m_image.width(), h = m_image.height();
    QImage scaledImage = m_image.scaled(w*scale, h*scale);
    label->setPixmap(QPixmap().fromImage(scaledImage));
}

void ImageViewer::zoomReset()
{
    scale = 1.0;
    label->setPixmap(QPixmap().fromImage(m_image));
}

void ImageViewer::setModified(bool modified)
{
    bool shouldEmit = m_modified == modified;
    m_modified = modified;
    if (shouldEmit)
        emit modificationChanged(modified);
}

void ImageViewer::savePreferences(const ImageSettingsDialog * dialog)
{
    m_settings->setValue("blpVersion", dialog->ImageSettingsDialog::blpVersion1->isChecked() ? 1 : 2);
    m_settings->setValue("blpType", dialog->ImageSettingsDialog::jpegCompressed->isChecked() ? 0 : 1);
    m_settings->setValue("blpCompression", dialog->ImageSettingsDialog::blp1Slider->value());
    m_settings->setValue("jpgCompression", dialog->ImageSettingsDialog::jpegSlider->value());
}

void ImageViewer::loadPreferences(ImageSettingsDialog * dialog)
{
    dialog->ImageSettingsDialog::blpVersion1->setChecked(m_settings->value("blpVersion").toInt() == 1 ? true : false);
    dialog->ImageSettingsDialog::blpVersion2->setChecked(m_settings->value("blpVersion").toInt() == 2 ? true : false);
    dialog->ImageSettingsDialog::jpegCompressed->setChecked(m_settings->value("blpType").toInt() == 0 ? true : false);
    dialog->ImageSettingsDialog::palettedImage->setChecked(m_settings->value("blpType").toInt() == 1 ? true : false);
    dialog->ImageSettingsDialog::blp1Slider->setValue(m_settings->value("blpCompression").toInt());
    dialog->ImageSettingsDialog::jpegSlider->setValue(m_settings->value("jpgCompression").toInt());
}
