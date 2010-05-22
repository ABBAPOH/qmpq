#include "mpqfileinfoiterator.h"

#include "qmpqarchive2.h"
#include "qmpqarchive2_p.h"

#include <QDebug>

MPQFileInfoIterator::MPQFileInfoIterator(QMPQArchive2 * archive, const QStringList & listfile, bool includeUnknowns) :
        m_archive(archive)
{
    mpq = m_archive->d_func()->mpq;
    m_listfile << "(listfile)" << "(attributes)";

    QString listfileString = m_archive->readFile("(listfile)");
    if (!listfileString.isEmpty()) {
        m_listfile.append(listfileString.split("\r\n"));
    }

    m_listfile.append(listfile);
    m_includeUnknowns = includeUnknowns;
    index = 0;
    listfileIndex = 0;
    hashIndex = 0;
    filesCount = m_archive->filesCount();
    hashTableSize = m_archive->hashTableSize();
}

//MPQFileInfoIterator::MPQFileInfoIterator()
//{
//}

MPQFileInfo MPQFileInfoIterator::getNext()
{
    MPQFileInfo result;
    for ( ; !result.isValid() && listfileIndex < m_listfile.size(); listfileIndex++) {
        result = m_archive->fileInfo(m_listfile.at(listfileIndex));
        if (result.isValid()) {
            indexes.append(result.blockIndex());
        }
    }

    if (result.isValid())
        return result;

    if (m_includeUnknowns)
    {
        for ( ; !result.isValid() && index < hashTableSize; index++) {
            if (indexes.contains(index)) {
                continue;
            }
            result = m_archive->fileInfo(index);
        }
    }
    return result;
}

bool MPQFileInfoIterator::hasNext()
{
    nextInfo = getNext();
    return nextInfo.isValid();
}

MPQFileInfo MPQFileInfoIterator::next()
{
    return nextInfo;
}
