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
        :QWidget(parent), ui(new Ui::ImageViewer), m_settings(new QSettings("QMPQ", "ImageViewer"))
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

//Maybe should create class for opening/saving fies
void ImageViewer::open(const QString & path)
{
    QString fileName = path;
    if(fileName == "")
        fileName = QFileDialog::getOpenFileName(this, tr("Select image file"));
    if (fileName == "")
        return;

    if (!fileName.isEmpty()) {
        m_currentFile = fileName;
        QImage image(fileName);
        m_currentImage = image;

        if (!image.isNull())
            label->setPixmap(QPixmap().fromImage(m_currentImage));
     }
}

void ImageViewer::closeFile()
{

}

void ImageViewer::save(QString path)
{
    qDebug() << "ImageViewer::save()" << path;
    if (path == "")
        path = currentFile();
    QImageWriter writer(path);
    QFileInfo info(path);

//    if (info.suffix().toLower() == "blp")
//        writer.setQuality(m_settings->value("blpCompression").toInt());
//    else if (info.suffix().toLower() == "jpg" || info.suffix().toLower() == "jpeg")
//        writer.setQuality(m_settings->value("jpgCompression").toInt());
    writer.setFormat("blp1jpeg");
    writer.write(m_currentImage);
}

void ImageViewer::preferences()
{
    ImageSettingsDialog dialog(this);
    loadPreferences(&dialog);
    dialog.exec();
    savePreferences(&dialog);
}

void ImageViewer::copy()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(m_currentImage);
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
