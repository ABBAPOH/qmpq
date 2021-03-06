#ifndef MPQEDITOR_H
#define MPQEDITOR_H

#include <QtGui/QWidget>
#include <QtCore/QModelIndex>

#include "universalview.h"

#define MaxViews 5

class MPQEditorError
{
public:
    MPQEditorError() {}
    explicit MPQEditorError(const QString errorString) { m_errorString = errorString; }
    QString errorString() const { return m_errorString; }
    void setErrorString(const QString errorString) { m_errorString = errorString; }
    void addSubError(const MPQEditorError &error) { m_suberrors.append( error); }
    QList<MPQEditorError> subErrors() const { return m_suberrors; }

private:
    QString m_errorString;
    QList<MPQEditorError> m_suberrors;
};

class QAbstractItemView;
class QListView;
class QColumnView;
class QTableView;
class QTreeView;
class FileSystemModel;
class QAbstractItemModel;
class QStackedLayout;
class QMPQFileEngine;
class UniversalView;
class QHBoxLayout;
class IDirModel;
class ArchiveSuffixesManager;
class MPQEditor : public QWidget
{
    Q_OBJECT
//    Q_ENUMS(ViewMode)
    enum ModelType{ Unknown, DirModel, FileSystemModel };

public:
    explicit MPQEditor(QWidget *parent = 0);
    ~MPQEditor();
//    enum ViewMode { ListView = 0, IconView, TableView, ColumnView, TreeView };

    QString currentFile() { return m_currentFile; }
    bool isMPQArchive(const QString & file);
    const MPQEditorError & lastError() const { return m_lastError; }
    void refresh(const QString & path);
    QStringList selectedPaths();
    ArchiveSuffixesManager * suffixesManager() const { return m_suffixesManager; }

    static QAbstractItemModel * model();

private:
	UniversalView * m_view;
    QHBoxLayout * m_layout;
    QString m_currentFile;
    QAction * openAction;
    IDirModel * m_model;
    ArchiveSuffixesManager * m_suffixesManager;
    MPQEditorError m_lastError;
    bool sendingEvent;

    ModelType getModelType(const QString & path);
    void initModel(const QString & path);
    void initViews();
    void resizeColumns();
    QModelIndexList selectedIndexes();
    QString selectedDir();
    void extract(const QString & path, const QString & destDir);
    void remove(const QModelIndex & path);

signals:
    void openRequested(const QString &file);
    void opened(const QString &file);
    void currentChanged(const QString &file);
    void errorOccured(const MPQEditorError & error);

public slots:
    void open(const QString &file);
    void reopenUsingListfile(const QByteArray &listfile);
    void closeFile();
    void add(const QStringList & files);
    void extract(const QString & destDir);
    void remove();
    void rename();
    void setViewMode(UniversalView::ViewMode mode);
    void newFolder(const QString & name = "");
    void setNameFilter(const QString & filter);
    void copy() ;
    void paste();
//    void remove();
private slots:
    void onDoubleClick(const QModelIndex & index);
    void onOpenRequest();
protected:
    void keyPressEvent ( QKeyEvent * event );
};

#endif // MPQEDITOR_H
