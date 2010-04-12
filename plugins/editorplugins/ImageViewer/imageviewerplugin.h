#ifndef MPQEDITORPLUGIN_H
#define MPQEDITORPLUGIN_H

#include <ieditorfactory.h>
#include <iplugin.h>

class ImageViewer;
class QSignalMapper;
class QAction;
class ImageViewerInterface : public IEditor
{
    Q_OBJECT
public:
    ImageViewerInterface(ImageViewer * editor);
    ~ImageViewerInterface();
    virtual QString currentFile() { return m_currentFile; }
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

class ImageViewerPlugin : public IPlugin
{
    Q_OBJECT
public:
    ImageViewerPlugin(QObject * parent = 0) : IPlugin(parent) {}
    virtual ~ImageViewerPlugin() {}

    virtual void initialize();
    virtual void shutdown() {};
    virtual QString name() { return "Image Viewer Plugin"; }
    virtual QString description() { return QString(); };

};

#endif // MPQEDITORPLUGIN_H
