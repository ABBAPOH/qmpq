#include "idirmodel.h"

//#include "QtGui/QApplication"

QFileSystemModel * FileSystemModelWrapper::m_model = 0;

IDirModel::IDirModel() {}
IDirModel::~IDirModel() {}

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
