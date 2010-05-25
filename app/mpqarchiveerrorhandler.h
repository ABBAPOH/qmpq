#ifndef MPQARCHIVEERRORHANDLER_H
#define MPQARCHIVEERRORHANDLER_H

#include <QObject>
#include <QtGui/QDialog>

#include "ui_errorsdialog.h"

class ErrorsDialog : public QDialog
{
    Q_OBJECT
    Ui::ErrorsDialog * ui;
public:
    explicit ErrorsDialog(QWidget * parent = 0);
    ~ErrorsDialog();

    void clearErrors();
    void addError(const QString & errString);
};

class QMPQArchive;
class MPQArchiveErrorHandler : public QObject
{
    Q_OBJECT
    ErrorsDialog * dialog;
    void onError(QMPQArchive * archive);
public:
    explicit MPQArchiveErrorHandler(QObject *parent = 0);
    ~MPQArchiveErrorHandler();

signals:

public slots:
    void onError();
    void addObject(QObject*);
};

#endif // MPQARCHIVEERRORHANDLER_H
