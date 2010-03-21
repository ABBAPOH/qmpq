#ifndef QMPQFILEENGINE_H
#define QMPQFILEENGINE_H

#include "QMPQFileEngine_global.h"

#include <QtCore/QAbstractFileEngine>
#include <QDebug>

class QMPQFileEngineStringParser
{
    bool m_found;
    QString m_fileName;
    QString m_suffix;

public:
    QMPQFileEngineStringParser(const QString & fileName, const QStringList suffixes)
    {
        m_found = false;
        qDebug() << fileName;
        const QStringList & names = fileName.split('/');
        //    foreach (const QString & name, names) {
        for (int i = names.count() - 1; i >= 0; i--) { // iterates list in a reverse order to support nested archives
            const QString & name = names.at(i);
            foreach (const QString & suffix, suffixes) {
//                qDebug() << suffix << fileName;
                if (name.endsWith('.' + suffix, Qt::CaseInsensitive)) {
                    m_found = true;
                    m_fileName = fileName;
                    m_suffix = suffix;
                    return;
//                    return new QMPQFileEngine(fileName);
                }
            }
        }
    }

    bool isFound() { return m_found; }
    QString fileName() { return m_fileName; }
    QString suffix() { return m_suffix; }
};

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
    bool	seek(qint64 offset);
    qint64 size() const;
    bool remove();
    bool rename(const QString & newName);
    bool rmdir(const QString & dirName, bool recurseParentDirectories) const;
    void setFileName(const QString & file);
    bool setPermissions(uint perms);
    bool setSize(qint64 size);
    qint64 write(const char * data, qint64 len);

    static QStringList supportedFormats()
    {
        return QStringList() << "mpq" << "w3x" << "w3m" << "s2ma" << "SC2Data" << "SC2Archive" << "SC2Assets"
                << "SC2Replay" << "scx" << "w3n" << "snp" << "sv" << "hsv";
    }
private:
    QFile * getNativeFile(QString path) const;
    QFileInfo * getNativeFileInfo(QString path) const;

protected:
    QMPQFileEnginePrivate * d_ptr;
};

#endif // QMPQFILEENGINE_H
