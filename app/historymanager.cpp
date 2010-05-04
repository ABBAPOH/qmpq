#include "historymanager.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDir>

HistoryManager::HistoryManager(QObject *parent) :
    QObject(parent), m_historyPos(-1)
{
}

void HistoryManager::back()
{
//    qDebug("EditorView::back");
//    qDebug() << m_history << historyPos;
    if (m_historyPos <= 0)
        return;
    m_historyPos--;
    QString path = m_history.at(m_historyPos);
    updatePath(path);
}

void HistoryManager::forward()
{
//    qDebug("EditorView::forward");
//    qDebug() << m_history << historyPos;
    if (m_historyPos == m_history.count() - 1)
        return;
    m_historyPos++;
    updatePath(m_history.at(m_historyPos));
}

void HistoryManager::up()
{
    QFileInfo info(m_path);
    if (info.isRoot())
        setPath("");
    else {
//        setPath(info.dir().path());
        setPath(info.path());
    }
}

//cleans all history from start to end of list
void HistoryManager::cleanHistory(int start)
{
//    qDebug("EditorView::cleanHistory");
    for (int i = start, end = m_history.count(); i < end; i++) {
        QString path = m_history.at(start);

        m_history.removeAt(start);
        emit pathRemoved(path);
    }
    m_historyPos = start - 1;
//    qDebug() << m_history;
}

void HistoryManager::updatePath(const QString & path)
{
    if (path == m_path)
        return;

    m_path = path;

    emit currentChanged(path);
}

void HistoryManager::setPath(const QString & pathIn)
{
    QString path = pathIn.endsWith('/') ? pathIn.left(pathIn.length()-1) : pathIn;
//    if (QFileInfo(m_path) == QFileInfo(path)) // ignores / in the end of the path
    if (path == m_path)
        return;

    if (m_historyPos != m_history.size() - 1) {
        cleanHistory(m_historyPos +1);
    }

    m_history.append(path);
    m_historyPos++;

    updatePath(path);

}
