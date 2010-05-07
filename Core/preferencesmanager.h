#ifndef PREFERENCESMANAGER_H
#define PREFERENCESMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include "ipreferences.h"

class QSettings;
class PreferencesManager : public IPreferences
{
Q_OBJECT
public:
    PreferencesManager(QObject * parent = 0);
    ~PreferencesManager();

    void addPreferencesPage(IPreferencesPage * preferencesPage);
    void loadSettings();
    QStringList keys();// { return QStringList(); }
    IPreferencesPage * page(const QString & key);
    QStringList pagesKeys();
    QVariant value(const QString & key);

private:
    QHash<QString, IPreferencesPage *> m_pages;
    QSettings * m_settings;
};

#endif // PREFERENCESMANAGER_H
