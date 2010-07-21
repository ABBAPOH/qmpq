#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QtGui/QWidget>
#include "ui_imageSettings.h"

class ImagePreferencesWidget : public QWidget {
    Q_OBJECT

public:
    explicit ImagePreferencesWidget(QWidget *parent = 0);
    ~ImagePreferencesWidget();

    int jpegQuality() { return ui->jpegSlider->value(); }
    void setjpegQuality(int value) { ui->jpegSlider->setValue(value); }

    QString BLPType();
    void setBLPType(const QString & type);

    int BLPjpegQuality() { return ui->blp1Slider->value(); }
    void setBLPjpegQuality(int value) { ui->blp1Slider->setValue(value); }

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ImageSettings *ui;

private slots:
};

#include <ipreferences.h>

class ImagePreferencesPage : public IPreferencesPage
{
public:
    explicit ImagePreferencesPage(QObject * parent = 0) :
            IPreferencesPage(parent),
            m_widget(new ImagePreferencesWidget)
    {
    }

    ~ImagePreferencesPage()
    {
#warning "check for leak"
//        delete m_widget;
    }

    QStringList keys();

//    void setDefaults();
    QVariant value(const QString & key);
    void setValue(const QString & key, const QVariant & value);

    virtual QString name() { return "Image Settings"; }
    virtual QString key() { return "ImageSettings"; }
    virtual QString groupKey() { return "ImageViewer"; }
    virtual QWidget * widget() { return m_widget; }

private:
    ImagePreferencesWidget * m_widget;
};

#endif // PREFERENCES_H
