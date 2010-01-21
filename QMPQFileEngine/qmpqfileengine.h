#ifndef QMPQFILEENGINE_H
#define QMPQFILEENGINE_H

#include "QMPQFileEngine_global.h"

#include <QtCore/QAbstractFileEngine>

class QMPQFileEnginePrivate;
class QMPQFILEENGINESHARED_EXPORT QMPQFileEngine : public QAbstractFileEngine
{
    Q_DECLARE_PRIVATE(QMPQFileEngine);
public:
    QMPQFileEngine();
    QMPQFileEngine(const QString & file);
    ~QMPQFileEngine();

    Iterator * beginEntryList(QDir::Filters filters, const QStringList & filterNames);
    bool caseSensitive () const;
    bool close();
    bool copy(const QString & newName);
    QStringList	entryList(QDir::Filters filters, const QStringList & filterNames) const;
    FileFlags fileFlags(FileFlags type = FileInfoAll) const ;
    QString fileName(FileName file = DefaultName) const;
    QDateTime fileTime(FileTime time) const;
    bool flush();
    bool isRelativePath() const;
//    bool isSequential() const { return true; } // Why works with it???
    bool mkdir(const QString & dirName, bool createParentDirectories) const;
    bool open(QIODevice::OpenMode mode);
    QString owner(FileOwner owner) const;
    uint ownerId(FileOwner owner) const;
    qint64 pos() const;
    qint64 read(char* data, qint64 maxlen);
    qint64 size() const;
    bool remove();
    bool rename(const QString & newName);
    bool rmdir(const QString & dirName, bool recurseParentDirectories) const;
    void setFileName(const QString & file);
    bool setPermissions(uint perms);
    bool setSize(qint64 size);
    qint64 write(const char * data, qint64 len);

    static QStringList supportedFormats() { return QStringList() << "mpq" << "w3x" << "w3m"; }
private:

protected:
    QMPQFileEnginePrivate * d_ptr;
};

#endif // QMPQFILEENGINE_H
