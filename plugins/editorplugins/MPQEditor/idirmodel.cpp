#include "idirmodel.h"

//#include "QtGui/QApplication"

QFileSystemModel * FileSystemModelWrapper::m_model = 0;

IDirModel::IDirModel() {}
IDirModel::~IDirModel() {}

DirModelWrapper::DirModelWrapper(const QString & path)
{
    m_model = new QDirModel(path);
    m_model->setReadOnly(false);
}

bool DirModelWrapper::remove(const QModelIndex & index) const
{
    qDebug() << "DirModelWrapper::remove" << m_model->filePath(index);
    bool result = true;
    if (m_model->isDir(index)) { // recursively removes current dir
        QDir dir(m_model->filePath(index));
        foreach (QString child, dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot)) {
            result &= remove(m_model->index(dir.filePath(child)));
        }
        
        //        for (int row = 0, rows = m_model->rowCount(index); row < rows; row++) {
        //            remove(index.child(0, 0));
        //        }
        //        m_model->in
        //        QApplication::processEvents();
        result &= m_model->rmdir(index);
    } else { // removes current file
        result = m_model->remove(index);
    }
    qDebug() << result;

    return result;
}
FileSystemModelWrapper::FileSystemModelWrapper()
{
    if (!m_model) {
        m_model = new QFileSystemModel;
        m_model->setRootPath("");
    }
}

QFileSystemModel * FileSystemModelWrapper::modelInstance()
{
    return m_model;
}