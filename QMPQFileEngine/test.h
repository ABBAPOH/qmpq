#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <qmpqarchive2.h>
#include <qmpqarchiveex.h>

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = 0);
    QMPQArchive2 * archive;
    QMPQArchiveEx * archiveEx;
    void testArchive();
    void testArchiveEx();

signals:

public slots:
    void archiveError();
    void addFileProgressChanged(quint32,quint32,bool);
    void compactProgressChanged(QMPQArchive2::CompactOperation,qint64,qint64);
};

#endif // TEST_H
