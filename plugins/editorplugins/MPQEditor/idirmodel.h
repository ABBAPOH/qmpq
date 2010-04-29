#ifndef IDIRMODEL
#define IDIRMODEL

#include <QtCore/QObject>
#include <QtCore/QModelIndex>
#include <QtCore/QString>

#include "../../../3rdParty/QDirModel/qdirmodel.h"
#include <QtGui/QFileSystemModel>

#include <QDebug>

class QAbstractItemModel;
class QDirModel;
class QFileSystemModel;
class IDirModel
{
public:
    IDirModel();
    virtual ~IDirModel();
    virtual QAbstractItemModel * model() = 0;
	virtual QModelIndex index(const QString & path, int column = 0) const = 0;
	virtual bool isDir(const QModelIndex & index) const = 0;
    virtual QString filePath(const QModelIndex & index) const = 0;
    virtual QModelIndex mkdir(const QModelIndex & parent, const QString & name) = 0;
    virtual bool remove(const QModelIndex & index) const = 0;
};

class DirModelWrapper : public IDirModel
{
public:
    explicit DirModelWrapper(const QString & path);
    ~DirModelWrapper() { delete m_model; }
	virtual QAbstractItemModel * model() { return m_model; }
	virtual QModelIndex index(const QString & path, int column = 0) const { return m_model->index(path, column); }
	virtual bool isDir(const QModelIndex & index) const { return m_model->isDir(index); }
    virtual QString filePath(const QModelIndex & index) const { return m_model->filePath(index); }
    virtual QModelIndex mkdir(const QModelIndex & parent, const QString & name) { return m_model->mkdir(parent, name); }
    virtual bool remove(const QModelIndex & index) const;
private:
	QDirModel * m_model;
};

class FileSystemModelWrapper : public IDirModel
{
public:
    FileSystemModelWrapper();
    ~FileSystemModelWrapper() { }
	virtual QAbstractItemModel * model() { return m_model; }
    virtual QModelIndex index(const QString & path, int column = 0) const { return m_model->index(path, column); }
	virtual bool isDir(const QModelIndex & index) const { return m_model->isDir(index); }
    virtual QString filePath(const QModelIndex & index) const { return m_model->filePath(index); }
    virtual QModelIndex mkdir(const QModelIndex & parent, const QString & name) { return m_model->mkdir(parent, name); }
    virtual bool remove(const QModelIndex & index) const { return m_model->remove(index); }
    static QFileSystemModel * modelInstance();
private:
    static QFileSystemModel * m_model;
};

//QT_BEGIN_NAMESPACE
//Q_DECLARE_INTERFACE(IDirModel, "IDirModel/1.0")
//QT_END_NAMESPACE

#endif // IDIRMODEL
