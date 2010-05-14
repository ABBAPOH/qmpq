#ifndef MPQEXTENSIONSSETTINGS_H
#define MPQEXTENSIONSSETTINGS_H

#include <QWidget>
#include <QModelIndex>
#include <ipreferences.h>

namespace Ui {
    class MPQExtensionsSettings;
}

class QStringListModel;
class MPQSettings;
class MPQExtensionsSettings : public QWidget
{
    Q_OBJECT
public:
    MPQExtensionsSettings(QWidget *parent = 0);
    ~MPQExtensionsSettings();
    void init();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MPQExtensionsSettings *ui;
    QStringListModel * model;
    QStringList extensions;
    MPQSettings * settings;

private slots:
    void addRow();
    void removeRow();
    void onDataChange(QModelIndex,QModelIndex);
};

class MPQExtensionsPage : public IPreferencesPage
{
public:
    explicit MPQExtensionsPage(QObject * parent = 0) : IPreferencesPage(parent), m_widget(new MPQExtensionsSettings)
    {
    }

    ~MPQExtensionsPage()
    {
    }

    QStringList keys();

    void setDefaults();
    QVariant value(const QString & key);
    void setValue(const QString & key, const QVariant & value);

    QString name() { return "MPQ"; }
    QString key() { return "MPQ"; }
    QWidget * widget() { return m_widget; }

private:
    MPQExtensionsSettings * m_widget;
};


#endif // MPQEXTENSIONSSETTINGS_H
