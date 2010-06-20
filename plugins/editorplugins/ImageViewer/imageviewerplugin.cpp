#include "imageviewerplugin.h"

#include <QtCore/QtPlugin>
#include <QtGui/QToolBar>
#include <QtGui/QFileDialog>
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
#include <QtGui/QImageWriter>
#include <QtGui/QMenu>
#include <QDebug>

#include "imageviewer.h"

#include "../../../Core/icore.h"

//================================== ImageViewerPlugin ==================================

ImageViewerInterface::ImageViewerInterface(ImageViewer * editor)
{
    m_editor = editor;
    m_toolBar = new QToolBar();

    initToolBar();
}

void ImageViewerInterface::initToolBar()
{
    IActionManager * manager = ICore::instance()->actionManager();
    m_toolBar->addAction(QIcon(":/icons/images/save.png"), "Save", this, SLOT(save()));
    m_toolBar->addSeparator();
    m_toolBar->addAction(QIcon(":/icons/images/copy.png"), "Copy", m_editor, SLOT(copy()));
    m_toolBar->addAction(QIcon(":/icons/images/paste.png"), "Paste", m_editor, SLOT(paste()));
    m_toolBar->addSeparator();
    m_toolBar->addAction(manager->action(Core::ACTION_ZOOM_IN));
    m_toolBar->addAction(manager->action(Core::ACTION_ZOOM_OUT));
    m_toolBar->addAction(manager->action(Core::ACTION_ZOOM_RESET));
}

ImageViewerInterface::~ImageViewerInterface()
{
}

bool ImageViewerInterface::open(const QString &file)
{
    if (!file.isEmpty()) {
        m_currentFile = file;
        QImage image(file);
        qDebug() << image.isNull();
        m_editor->setImage(image);
        return true;
    }

    return true;
}

void ImageViewerInterface::close()
{
    m_editor->clear();
}

QWidget * ImageViewerInterface::widget()
{
    return m_editor;
}

QToolBar * ImageViewerInterface::toolBar ()
{
    return m_toolBar;
}

void ImageViewerInterface::save(const QString &file)
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

IEditor * ImageViewerFactory::createEditor(QWidget * parent)
{
    ImageViewer * editor = new ImageViewer(parent);
    return new ImageViewerInterface(editor);
}

QIcon ImageViewerFactory::icon() const
{
    return QIcon();
}

void ImageViewerFactory::shutdown()
{
}

//================================== ImageViewerPlugin ==================================

void ImageViewerPlugin::initialize()
{
    ICore * core = ICore::instance();

    core->editorFactoryManager()->addFactory(new ImageViewerFactory);
    core->fileManager()->registerExtensionString("Image Files (*.blp *.tga *.jpg *.bmp *.png)");

    QAction * actionZoomIn = new QAction(QIcon(":/icons/images/zoomin.png"), tr("Zoom In"), this);
    QAction * actionZoomOut = new QAction(QIcon(":/icons/images/zoomout.png"), tr("Zoom Out"), this);
    QAction * actionZoomReset = new QAction(QIcon(":/icons/images/resetzoom.png"), tr("Reset Zoom"), this);

    connect(actionZoomIn, SIGNAL(triggered()), SLOT(zoomIn()));
    connect(actionZoomOut, SIGNAL(triggered()), SLOT(zoomOut()));
    connect(actionZoomReset, SIGNAL(triggered()), SLOT(zoomReset()));

    actionZoomIn->setShortcut(tr("Ctrl+="));
    actionZoomOut->setShortcut(tr("Ctrl+-"));
    actionZoomReset->setShortcut(tr("Ctrl+0"));

    QMenu * menuTools = core->actionManager()->menu(Core::MENU_TOOLS);
    QMenu * menu = menuTools->addMenu(tr("Image Viewer"));
    menu->addAction(actionZoomIn);
    menu->addAction(actionZoomOut);
    menu->addAction(actionZoomReset);
    core->actionManager()->registerAction(actionZoomIn, Core::ACTION_ZOOM_IN);
    core->actionManager()->registerAction(actionZoomOut, Core::ACTION_ZOOM_OUT);
    core->actionManager()->registerAction(actionZoomReset, Core::ACTION_ZOOM_RESET);

    connect(core->editorFactoryManager(), SIGNAL(currentEditorChanged(IEditor*)), SLOT(updateActions()));
}

ImageViewerInterface * ImageViewerPlugin::editor()
{
    ICore * core = ICore::instance();
    return qobject_cast<ImageViewerInterface *>(core->editorFactoryManager()->currentEditor());
}

ImageViewer * ImageViewerPlugin::editorWidget()
{
    ImageViewerInterface * editor = this->editor();
    if (editor)
        return static_cast<ImageViewer*>(editor->widget());
    return 0;
}

void ImageViewerPlugin::updateActions()
{
    ICore * core = ICore::instance();
    ImageViewerInterface * editor = this->editor();
    core->actionManager()->action(Core::ACTION_ZOOM_IN)->setEnabled(editor);;
    core->actionManager()->action(Core::ACTION_ZOOM_OUT)->setEnabled(editor);;
    core->actionManager()->action(Core::ACTION_ZOOM_RESET)->setEnabled(editor);;
}

void ImageViewerPlugin::zoomIn()
{
    ImageViewer * viewer = editorWidget();
    if (viewer)
        viewer->zoomIn();
}

void ImageViewerPlugin::zoomOut()
{
    ImageViewer * viewer = editorWidget();
    if (viewer)
        viewer->zoomOut();
}

void ImageViewerPlugin::zoomReset()
{
    ImageViewer * viewer = editorWidget();
    if (viewer)
        viewer->zoomReset();
}

//Q_EXPORT_PLUGIN2(image_viewer_factory, ImageViewerFactory)
Q_EXPORT_PLUGIN(ImageViewerPlugin)
