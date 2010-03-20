/********************************************************************************
** Form generated from reading UI file 'imageSettingsDialog.ui'
**
** Created: Sun Mar 21 00:18:52 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGESETTINGSDIALOG_H
#define UI_IMAGESETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ImageSettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *jpegGroupBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *jpegQualityLabel;
    QSlider *jpegSlider;
    QSpinBox *jpegSpinBox;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *blpVersion1;
    QRadioButton *blpVersion2;
    QGroupBox *blp1GroupBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *jpegCompressed;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout;
    QLabel *blp1QualityLabel;
    QSlider *blp1Slider;
    QSpinBox *blp1SpinBox;
    QRadioButton *palettedImage;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *useAplphaList;
    QGroupBox *blp2groupBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ImageSettingsDialog)
    {
        if (ImageSettingsDialog->objectName().isEmpty())
            ImageSettingsDialog->setObjectName(QString::fromUtf8("ImageSettingsDialog"));
        ImageSettingsDialog->resize(480, 640);
        ImageSettingsDialog->setModal(false);
        verticalLayout = new QVBoxLayout(ImageSettingsDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        jpegGroupBox = new QGroupBox(ImageSettingsDialog);
        jpegGroupBox->setObjectName(QString::fromUtf8("jpegGroupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(jpegGroupBox->sizePolicy().hasHeightForWidth());
        jpegGroupBox->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(jpegGroupBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        jpegQualityLabel = new QLabel(jpegGroupBox);
        jpegQualityLabel->setObjectName(QString::fromUtf8("jpegQualityLabel"));

        horizontalLayout_3->addWidget(jpegQualityLabel);

        jpegSlider = new QSlider(jpegGroupBox);
        jpegSlider->setObjectName(QString::fromUtf8("jpegSlider"));
        jpegSlider->setMinimum(1);
        jpegSlider->setMaximum(100);
        jpegSlider->setValue(75);
        jpegSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(jpegSlider);

        jpegSpinBox = new QSpinBox(jpegGroupBox);
        jpegSpinBox->setObjectName(QString::fromUtf8("jpegSpinBox"));
        jpegSpinBox->setMinimum(1);
        jpegSpinBox->setMaximum(100);
        jpegSpinBox->setValue(75);

        horizontalLayout_3->addWidget(jpegSpinBox);


        verticalLayout->addWidget(jpegGroupBox);

        groupBox = new QGroupBox(ImageSettingsDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        blpVersion1 = new QRadioButton(groupBox);
        blpVersion1->setObjectName(QString::fromUtf8("blpVersion1"));
        blpVersion1->setChecked(true);

        horizontalLayout_2->addWidget(blpVersion1);

        blpVersion2 = new QRadioButton(groupBox);
        blpVersion2->setObjectName(QString::fromUtf8("blpVersion2"));
        blpVersion2->setEnabled(false);

        horizontalLayout_2->addWidget(blpVersion2);


        verticalLayout->addWidget(groupBox);

        blp1GroupBox = new QGroupBox(ImageSettingsDialog);
        blp1GroupBox->setObjectName(QString::fromUtf8("blp1GroupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(blp1GroupBox->sizePolicy().hasHeightForWidth());
        blp1GroupBox->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(blp1GroupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        jpegCompressed = new QRadioButton(blp1GroupBox);
        jpegCompressed->setObjectName(QString::fromUtf8("jpegCompressed"));
        jpegCompressed->setChecked(true);

        verticalLayout_2->addWidget(jpegCompressed);

        frame_3 = new QFrame(blp1GroupBox);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        blp1QualityLabel = new QLabel(frame_3);
        blp1QualityLabel->setObjectName(QString::fromUtf8("blp1QualityLabel"));

        horizontalLayout->addWidget(blp1QualityLabel);

        blp1Slider = new QSlider(frame_3);
        blp1Slider->setObjectName(QString::fromUtf8("blp1Slider"));
        blp1Slider->setMinimum(1);
        blp1Slider->setMaximum(100);
        blp1Slider->setValue(75);
        blp1Slider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(blp1Slider);

        blp1SpinBox = new QSpinBox(frame_3);
        blp1SpinBox->setObjectName(QString::fromUtf8("blp1SpinBox"));
        blp1SpinBox->setMinimum(1);
        blp1SpinBox->setMaximum(100);
        blp1SpinBox->setValue(75);

        horizontalLayout->addWidget(blp1SpinBox);


        verticalLayout_2->addWidget(frame_3);

        palettedImage = new QRadioButton(blp1GroupBox);
        palettedImage->setObjectName(QString::fromUtf8("palettedImage"));
        palettedImage->setEnabled(false);

        verticalLayout_2->addWidget(palettedImage);

        frame_4 = new QFrame(blp1GroupBox);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setFrameShape(QFrame::NoFrame);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_4);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        useAplphaList = new QCheckBox(frame_4);
        useAplphaList->setObjectName(QString::fromUtf8("useAplphaList"));
        useAplphaList->setEnabled(false);

        verticalLayout_4->addWidget(useAplphaList);


        verticalLayout_2->addWidget(frame_4);


        verticalLayout->addWidget(blp1GroupBox);

        blp2groupBox = new QGroupBox(ImageSettingsDialog);
        blp2groupBox->setObjectName(QString::fromUtf8("blp2groupBox"));
        sizePolicy1.setHeightForWidth(blp2groupBox->sizePolicy().hasHeightForWidth());
        blp2groupBox->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(blp2groupBox);

        buttonBox = new QDialogButtonBox(ImageSettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ImageSettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ImageSettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ImageSettingsDialog, SLOT(reject()));
        QObject::connect(blp1Slider, SIGNAL(valueChanged(int)), blp1SpinBox, SLOT(setValue(int)));
        QObject::connect(blp1SpinBox, SIGNAL(valueChanged(int)), blp1Slider, SLOT(setValue(int)));
        QObject::connect(jpegSlider, SIGNAL(valueChanged(int)), jpegSpinBox, SLOT(setValue(int)));
        QObject::connect(jpegSpinBox, SIGNAL(valueChanged(int)), jpegSlider, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(ImageSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *ImageSettingsDialog)
    {
        ImageSettingsDialog->setWindowTitle(QApplication::translate("ImageSettingsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        jpegGroupBox->setTitle(QApplication::translate("ImageSettingsDialog", "Jpeg Options", 0, QApplication::UnicodeUTF8));
        jpegQualityLabel->setText(QApplication::translate("ImageSettingsDialog", "Quality:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ImageSettingsDialog", "Blp Version", 0, QApplication::UnicodeUTF8));
        blpVersion1->setText(QApplication::translate("ImageSettingsDialog", "BLP1", 0, QApplication::UnicodeUTF8));
        blpVersion2->setText(QApplication::translate("ImageSettingsDialog", "BLP2", 0, QApplication::UnicodeUTF8));
        blp1GroupBox->setTitle(QApplication::translate("ImageSettingsDialog", "Blp1 Options", 0, QApplication::UnicodeUTF8));
        jpegCompressed->setText(QApplication::translate("ImageSettingsDialog", "Jpeg Compressed", 0, QApplication::UnicodeUTF8));
        blp1QualityLabel->setText(QApplication::translate("ImageSettingsDialog", "Quality:", 0, QApplication::UnicodeUTF8));
        palettedImage->setText(QApplication::translate("ImageSettingsDialog", "Palletted Image", 0, QApplication::UnicodeUTF8));
        useAplphaList->setText(QApplication::translate("ImageSettingsDialog", "Use alpha list (more quality)", 0, QApplication::UnicodeUTF8));
        blp2groupBox->setTitle(QApplication::translate("ImageSettingsDialog", "Blp2 Options", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImageSettingsDialog: public Ui_ImageSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGESETTINGSDIALOG_H
