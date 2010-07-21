#include "preferences.h"

ImagePreferencesWidget::ImagePreferencesWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::ImageSettings)
{
    ui->setupUi(this);
}

ImagePreferencesWidget::~ImagePreferencesWidget()
{

}

QString ImagePreferencesWidget::BLPType()
{
    if (ui->jpegCompressed->isChecked())
        return "blp1jpg";
    if (ui->palettedImage->isChecked())
        return "blp1pal";
}

void ImagePreferencesWidget::setBLPType(const QString & type)
{
    if (type == "blp1jpg") {
        ui->jpegCompressed->setChecked(true);
        ui->palettedImage->setChecked(false);
    }
    if (type == "blp1pal") {
        ui->jpegCompressed->setChecked(false);
        ui->palettedImage->setChecked(true);
    }
}

void ImagePreferencesWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


QStringList ImagePreferencesPage::keys()
{
    return QStringList() << "jpegquality" << "blptype" << "blpjpegquality";
}

QVariant ImagePreferencesPage::value(const QString & key)
{
    if (key == "jpegquality")
        return m_widget->jpegQuality();
    if (key == "blptype")
        return m_widget->BLPType();
    if (key == "blpjpegquality")
        return m_widget->BLPjpegQuality();
}

void ImagePreferencesPage::setValue(const QString & key, const QVariant & value)
{
    if (key == "jpegquality")
        m_widget->setjpegQuality(value.toInt());
    if (key == "blptype")
        m_widget->setBLPType(value.toString());
    if (key == "blpjpegquality")
        m_widget->setBLPjpegQuality(value.toInt());
}

