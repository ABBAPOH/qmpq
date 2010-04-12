#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

class HistoryManager : public QObject
{
Q_OBJECT
Q_PROPERTY(QString path READ path WRITE setPath)
public:
    explicit HistoryManager(QObject *parent = 0);
    QString path() { return m_path; }

signals:
   void currentChanged(const QString &);
   void pathRemoved(const QString &);

public slots:
    void back();
    void forward();
    void up();
    void setPath(const QString & path);

private:
    void cleanHistory(int start);
    void updatePath(const QString & path);

    QString m_path;
    QStringList m_history;
    int m_historyPos;
};

#endif // HISTORYMANAGER_H
