#ifndef ARCHIVESUFFIXESMANAGER_H
#define ARCHIVESUFFIXESMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>
#include <QtCore/QMap>

class ArchiveSuffixesManager : public QObject
{
    Q_OBJECT
public:
    void addSuffix(const QString & suffix, const QString & prefix)
    {
        m_prefixes.insert(suffix, prefix);
    }

    void addSuffixes(const QStringList & suffixes, const QString & prefix)
    {
        foreach (QString suffix, suffixes) {
            addSuffix(suffix, prefix);
        }
    }

    QString mapFromPath(QString path) const
    {
        QFileInfo info(path);
        return m_prefixes.value(info.suffix());
    }

private:
    QMap<QString, QString> m_prefixes;
};

#endif // ARCHIVESUFFIXESMANAGER_H
