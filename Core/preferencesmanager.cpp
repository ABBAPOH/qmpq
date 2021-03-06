#include "preferencesmanager.h"

#include <QtCore/QVariant>
#include <QSettings>

#include <QDebug>

PreferencesManager::PreferencesManager(QObject * parent)
    : IPreferences(parent)
{
    m_settings = new QSettings("QMPQ", "app", this);
}

//void PreferencesManager::loadSettings()
//{
//    QSettings settings("QMPQ", "app", this);
//    foreach (QString key, keys) {
////        qDebug() << key << value(key);
//        int index = key.lastIndexOf('/');
//        QString pageKey;
//        QString shortKey;
////        if (index != -1)
//            pageKey = key.left(index);
//            shortKey = key.mid(index + 1);
////        else
////            pageKey = key;
//        IPreferencesPage * preferencesPage = m_pages.value(pageKey);
//        if (preferencesPage)
//            preferencesPage->setValue(shortKey, settings.value(key));
//    }
//}

PreferencesManager::~PreferencesManager()
{
    m_settings->clear();
    foreach (QString key, keys()) {
//        qDebug() << key << value(key);
        QVariant val = value(key);
        m_settings->setValue(key, val);
    }
}

QStringList PreferencesManager::keys()
{
    QStringList keys;
    foreach (IPreferencesPage * preferencesPage, m_pages) {
        foreach (QString key, preferencesPage->keys()) {
            keys.append(preferencesPage->groupKey() + '/' + preferencesPage->key() + '/' + key);
        }
    }
    return keys;
}

void PreferencesManager::addPreferencesPage(IPreferencesPage * preferencesPage)
{
    QString groupKey = preferencesPage->groupKey();
    QString fullKey = groupKey + '/' + preferencesPage->key();
    m_pagesList.append(preferencesPage);
    m_pages.insert(fullKey, preferencesPage);

    m_settings->beginGroup(fullKey);
    QStringList keys = m_settings->childKeys();
    if (keys.isEmpty()) { // we set default values
        preferencesPage->setDefaults();
    } else { // we read them from QSettings
        foreach (QString key, m_settings->childKeys()) {
//            qDebug() << key;
            preferencesPage->setValue(key, m_settings->value(key));
        }
    }
    m_settings->endGroup();
}

IPreferencesPage * PreferencesManager::page(const QString & groupKey, const QString & key)
{
    return m_pages.value(groupKey + '/' + key);
}

QList<IPreferencesPage *> PreferencesManager::pages()
{
//    return m_pages.values();
    //  i return list to make sure pages will be in order they were added
    return m_pagesList;
}

QStringList PreferencesManager::pagesKeys()
{
    return m_pages.keys();
}

QVariant PreferencesManager::value(const QString & key)
{
    int index = key.lastIndexOf('/');
    Q_ASSERT(index != -1);
    QString pageKey = key.mid(0, index);
    QString shortKey = key.mid(index + 1);
    return m_pages.value(pageKey)->value(shortKey);
}
