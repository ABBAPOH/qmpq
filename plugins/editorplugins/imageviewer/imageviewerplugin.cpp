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

void ImageViewerPlugin::save()
{
    m_editor->save(m_editor->currentFile());
}

void ImageViewerPlugin::save_As()
{
    QString path = QFileDialog::getSaveFileName(m_editor, tr("Save As..."), m_editor->currentFile(),  tr("Images (*.blp *.bmp *.tga *.png *.xpm *.jpg)"));
    if (path == "")
        return;
    m_editor->save(path);
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

