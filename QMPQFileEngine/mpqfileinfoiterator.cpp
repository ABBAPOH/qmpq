#include "mpqfileinfoiterator.h"

#include "qmpqarchive.h"
#include "qmpqarchive_p.h"

#include <StormLib/StormLib.h>

MPQFileInfoIterator::MPQFileInfoIterator(QMPQArchive * archive, const QStringList & listfile, bool includeUnknowns) :
        m_archive(archive),
        m_includeUnknowns(includeUnknowns)
{
    mpq = m_archive->d_func()->mpq;

    SFILE_FIND_DATA sf;
//    HANDLE hFind;

    hFind = SFileFindFirstFile(mpq, "*", &sf, 0);

    searching = true;

    m_archive->setLocale(m_archive->getLocale(sf.lcLocale)); // we open file in correct locale
    currentInfo = m_archive->fileInfo(sf.cFileName);
    m_archive->setLocale(QLocale(QLocale::C));

    indexes.append(currentInfo.blockIndex());
    index = 0;
    filesFound = 1;
    hashTableSize = m_archive->blockTableSize();
    filesCount = m_archive->filesCount();
}

MPQFileInfoIterator::~MPQFileInfoIterator()
{
    SFileFindClose(hFind);
}

bool MPQFileInfoIterator::hasNext()
{
    return (hFind != 0 && searching) || (m_includeUnknowns && (filesFound <= filesCount));
}

MPQFileInfo MPQFileInfoIterator::next()
{
    MPQFileInfo info = currentInfo;
    if (searching) {
        SFILE_FIND_DATA sf;
        searching = SFileFindNextFile(hFind, &sf);
        if (searching) {
            filesFound++;

            m_archive->setLocale(m_archive->getLocale(sf.lcLocale)); // we open file in correct locale
            currentInfo = m_archive->fileInfo(sf.cFileName);
            m_archive->setLocale(QLocale(QLocale::C));

            indexes.append(currentInfo.blockIndex());
        }
    } else {
        currentInfo = MPQFileInfo();
        for ( ; !currentInfo.isValid() && index < hashTableSize; index++) {
            if (indexes.contains(index)) {
                continue;
            }
            currentInfo = m_archive->fileInfo(index);
        }
        filesFound++;
    }

    return info;
}
