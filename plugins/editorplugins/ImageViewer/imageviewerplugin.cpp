#include "imageviewerplugin.h"

#include <QtCore/QtPlugin>
#include <QtGui/QToolBar>
#include <QtGui/QFileDialog>
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
#include <QtGui/QImageWriter>
#include <QDebug>

#include "imageviewer.h"

//================================== ImageViewerPlugin ==================================

ImageViewerPlugin::ImageViewerPlugin(ImageViewer * editor)
{
    m_editor = editor;
    m_toolBar = new QToolBar();

    initToolBar();
}

void ImageViewerPlugin::initToolBar()
{
    m_toolBar->addAction(QIcon(":/icons/images/save.png"), "Save", this, SLOT(save()));
    m_toolBar->addSeparator();
    m_toolBar->addAction(QIcon(":/icons/images/copy.png"), "Copy", m_editor, SLOT(copy()));
    m_toolBar->addAction(QIcon(":/icons/images/paste.png"), "Paste", m_editor, SLOT(paste()));
    m_toolBar->addSeparator();
    m_toolBar->addAction(QIcon(":/icons/images/zoomin.png"), "Zoom In", m_editor, SLOT(zoomIn()));
    m_toolBar->addAction(QIcon(":/icons/images/zoomout.png"), "Zoom Out", m_editor, SLOT(zoomOut()));
    m_toolBar->addAction(QIcon(":/icons/images/resetzoom.png"), "Reset Zoom", m_editor, SLOT(zoomReset()));
}

ImageViewerPlugin::~ImageViewerPlugin()
{
}

bool ImageViewerPlugin::open(const QString &file)
{
    qDebug() << "ImageViewerPlugin::open" << file;
    if (!file.isEmpty()) {
        m_currentFile = file;
        QImage image(file);
        qDebug() << image.isNull();
        m_editor->setImage(image);
        return true;
    }

    return true;
}

void ImageViewerPlugin::close()
{
    m_editor->clear();
}

QWidget * ImageViewerPlugin::widget()
{
    return m_editor;
}

QToolBar * ImageViewerPlugin::toolBar ()
{
    return m_toolBar;
}

void ImageViewerPlugin::save(const QString &file)
{
    QString path = file ==  "" ? m_currentFile : file;

    //    qDebug() << "ImageViewer::save()" << path;
    //    if (path == "")
    //        path = currentFile();
    QImageWriter writer(path);
    QFileInfo info(path);

    //    if (info.suffix().toLower() == "blp")
    //        writer.setQuality(m_settings->value("blpCompression").toInt());
    //    else if (info.suffix().toLower() == "jpg" || info.suffix().toLower() == "jpeg")
    //        writer.setQuality(m_settings->value("jpgCompression").toInt());
    writer.setFormat("blp1jpeg");
    writer.write(m_editor->image());
    m_editor->setModified(false);
}

//================================== ImageViewerFactory ==================================

ImageViewerFactory::ImageViewerFactory()
{

}

IEditor * ImageViewerFactory::createEditor(QWidget * parent)
{
    ImageViewer * editor = new ImageViewer(parent);
    return new ImageViewerPlugin(editor);
}

void ImageViewerFactory::shutdown()
{
}

bool ImageViewerFactory::canHandle(const QString &file) const
{
    QStringList extensions;
    QString suffix = QFileInfo(file).suffix().toLower();
    extensions << "blp" << "tga" << "jpg" << "bmp" << "png";
    foreach (QString extension, extensions)
        if (suffix == extension)
            return true;
    return false;
}

Q_EXPORT_PLUGIN2(image_viewer_factory, ImageViewerFactory)

