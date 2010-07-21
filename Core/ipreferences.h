#ifndef IPREFERENCES_H
#define IPREFERENCES_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

class IPreferences : public QObject
{
    Q_OBJECT

public:
    IPreferences(QObject * parent = 0) : QObject(parent) {}
    virtual ~IPreferences() {}
    virtual QStringList keys() = 0;
    virtual QVariant value(const QString & key) = 0;

signals:
    void settingChanged(const QString & key);
};

class IPreferencesPage : public IPreferences
{
public:
    IPreferencesPage(QObject * parent = 0) : IPreferences(parent) {}
    ~IPreferencesPage() {}
    virtual void setDefaults() {}
    virtual void setValue(const QString & key, const QVariant & value) = 0;
    virtual QString name() = 0;
    virtual QString groupKey() = 0;
    virtual QString key() = 0;
    virtual QWidget * widget() = 0;
};

#endif // IPREFERENCES_H
