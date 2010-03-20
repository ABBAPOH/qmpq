#ifndef MPQEDITORPLUGIN_H
#define MPQEDITORPLUGIN_H

#include "../../../app/ieditorfactory.h"

class ImageViewer;
class QSignalMapper;
class QAction;
class ImageViewerPlugin : public QObject, public IEditor
{
    Q_OBJECT
    Q_INTERFACES(IEditor)
public:
    ImageViewerPlugin(ImageViewer * editor);
    ~ImageViewerPlugin();
    virtual bool open(const QString & file);
    virtual QWidget * widget();
    virtual QToolBar * toolBar ();
    virtual void close();
    virtual void save(const QString & file);

private:
    ImageViewer * m_editor;
    QToolBar * m_toolBar;
    QAction * viewModeActions[5];
    QString m_currentFile;

    void initToolBar();

//    void save_As();
};

class ImageViewerFactory : public QObject, public IEditorFactory
{
    Q_OBJECT
    Q_INTERFACES(IEditorFactory)
public:
    IEditor * createEditor(QWidget * parent);
    void shutdown();
    bool canHandle(const QString &file) const;
    virtual QString saveFilter() const { return tr("Images (*.blp *.bmp *.tga *.png *.xpm *.jpg)"); }

    ImageViewerFactory();
};

#endif // MPQEDITORPLUGIN_H
