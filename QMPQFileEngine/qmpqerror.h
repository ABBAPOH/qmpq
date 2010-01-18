#ifndef QMPQERROR_H
#define QMPQERROR_H

#include <QString>

class QMPQError {
public:
    const unsigned errorCode() const { return m_errorCode; }
    const QString & errorMessage() const { return m_errorMessage; }
    int operator= (unsigned value) { m_errorCode = value; m_errorMessage = ErrString(value); return m_errorCode; }
    bool operator== (unsigned value) { return m_errorCode == value; }

private:
    const char *ErrString(int err);

    unsigned m_errorCode;
    QString m_errorMessage;
};

#endif // QMPQERROR_H
