#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <qmpqarchive.h>
#include <qmpqarchiveex.h>

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = 0);
    QMPQArchive * archive;
    QMPQArchiveEx * archiveEx;
    void testArchive(const QString & archiveName);
    void testArchiveEx(const QString & archiveName);
    void testEngine(const QString & archiveName);

signals:

public slots:
    void archiveError();
    void addFileProgressChanged(quint32,quint32,bool);
    void compactProgressChanged(QMPQArchive::CompactOperation,qint64,qint64);
};

#endif // TEST_H
