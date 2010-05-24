#ifndef POWERSPINBOX_H
#define POWERSPINBOX_H

#include <QSpinBox>

class PowerSpinBox : public QSpinBox
{
    Q_OBJECT
    Q_PROPERTY(int base READ base WRITE setBase);
    Q_PROPERTY(int power READ power WRITE setPower);

    int getPow(int value) const;

public:
    explicit PowerSpinBox(QWidget *parent = 0);

    int base() const { return m_base; }
    void setBase(int base) { m_base = base; }

    int power() const { return m_power; }
    void setPower(int power) { m_power = power; }

protected:
    void fixup(QString & input) const;
    void stepBy(int steps);
//    QValidator::State validate(QString & text, int & pos) const;

signals:

public slots:

private:
    int m_base;
    int m_power;
};

#endif // POWERSPINBOX_H
