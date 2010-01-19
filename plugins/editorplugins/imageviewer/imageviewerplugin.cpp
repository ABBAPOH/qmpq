#include "imageviewerplugin.h"

#include <QtCore/QtPlugin>
#include <QtGui/QToolBar>
#include <QtGui/QFileDialog>
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
#include <QDebug>

#include "imageviewer.h"

//================================== ImageViewerPlugin ==================================

ImageViewerPlugin::ImageViewerPlugin(ImageViewer * editor)
{
    m_editor = editor;
    m_toolBar = new QToolBar();


}

ImageViewerPlugin::~ImageViewerPlugin()
{
}

bool ImageViewerPlugin::open(const QString &file)
{
    m_editor->open(file);
    return true;
}

void ImageViewerPlugin::close()
{
    m_editor->closeFile();
}

QWidget * ImageViewerPlugin::widget()
{
    return m_editor;
}

QToolBar * ImageViewerPlugin::toolBar ()
{
    return m_toolBar;
}

//================================== ImageViewerFactory ==================================

ImageViewerFactory::ImageViewerFactory()
{

}

IEditor * ImageViewerFactory::createEditor(QWidget * parent)
{
//    qDebug("ImageViewerFactory::instance");
    ImageViewer * editor = new ImageViewer(parent);
    return new ImageViewerPlugin(editor);
}

void ImageViewerFactory::shutdown()
{
//    qDebug("ImageViewerFactory::shutdown");
//    delete MPQEditor::model();
}

bool ImageViewerFactory::canHandle(const QString &file) const
{
//    qDebug("ImageViewerFactory::canHandle");
    QStringList extensions;
    QString suffix = QFileInfo(file).suffix().toLower();
    extensions << "blp" << "tga" << "jpg" << "bmp" << "png";
    foreach (QString extension, extensions)
        if (suffix == extension)
            return true;
    return false;
}

Q_EXPORT_PLUGIN2(image_viewer_factory, ImageViewerFactory)

