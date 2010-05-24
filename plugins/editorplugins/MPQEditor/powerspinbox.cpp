#include "powerspinbox.h"

#include <QtGui/QLineEdit>
#include <math.h>

#include <QDebug>

PowerSpinBox::PowerSpinBox(QWidget *parent) :
    QSpinBox(parent),
    m_base(2),
    m_power(0)
{
    setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
    lineEdit()->setReadOnly(true);
}

int PowerSpinBox::getPow(int value) const
{
    if (value == 0)
        return 0;
    return log(value)/log(base());
}

void PowerSpinBox::fixup(QString & input) const
{
    int value = input.toInt();
    int power = getPow(value);
    input = QString::number((int)::pow(base(), power));
}

void PowerSpinBox::stepBy(int steps)
{
    int newValue = 0;

    m_power = getPow(value()) + steps;

    newValue = pow(base(), power());
    if (newValue > maximum())
        newValue = maximum();
    if (newValue < minimum())
        newValue = minimum();
    setValue(newValue);
}

//QValidator::State PowerSpinBox::validate(QString & text, int & pos) const
//{
//    bool ok;
//    int value = text.toInt(&ok);
//    int pow = getPow(value);
////    qDebug() << text << pos << value << pow;
//    if (!ok && text != "")
//        return QValidator::Invalid;
//    if ((int)::pow(base(), pow) != value) {
//        return QValidator::Intermediate;
//    }
//    return QValidator::Acceptable;
//}
