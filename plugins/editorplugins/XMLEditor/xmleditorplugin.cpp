#include "xmleditorplugin.h"

#include <QtCore/QFileInfo>
#include <QtCore/QtPlugin>
#include <QtGui/QToolBar>

#include <qxmlviewer.h>

//================================== XMLEditorPluginPlugin ==================================

XMLEditorInterface::XMLEditorInterface(QXMLViewer * editor)
{
    m_editor = editor;
    m_toolBar = new QToolBar();

//    initToolBar();
}

XMLEditorInterface::~XMLEditorInterface()
{
}

bool XMLEditorInterface::open(const QString &filePath)
{
    if (filePath == "")
        return false;
    m_currentFile = filePath;
    QFile file(filePath);
    return m_editor->setContent(&file);
}

//void XMLEditorPlugin::save(const QString &file)
//{
//
//}

void XMLEditorInterface::close()
{
}

QWidget * XMLEditorInterface::widget()
{
    return m_editor;
}

QToolBar * XMLEditorInterface::toolBar ()
{
    return m_toolBar;
}

//================================== XMLEditorFactory ==================================

XMLEditorFactory::XMLEditorFactory()
{

}

bool XMLEditorFactory::canHandle(const QString &file) const
{
    QStringList extensions;
    QString suffix = QFileInfo(file).suffix().toLower();
    extensions << "xml" << "ui";
    foreach (QString extension, extensions)
        if (suffix == extension)
            return true;
    return false;
}

IEditor * XMLEditorFactory::createEditor(QWidget * parent)
{
//    qDebug("XMLEditorFactory::instance");
    QXMLViewer * editor = new QXMLViewer(parent);
    return new XMLEditorInterface(editor);
}

QIcon XMLEditorFactory::icon() const
{
    return QIcon();
}

void XMLEditorFactory::shutdown()
{
//    qDebug("XMLEditorFactory::shutdown");
//    delete MPQEditor::model();
}

//================================== XMLEditorPlugin ==================================

void XMLEditorPlugin::initialize()
{
    ICore::instance()->editorFactoryManager()->addFactory(new XMLEditorFactory);
    ICore::instance()->fileManager()->registerExtensionString("XML Files (*.xml *.ui)");
}

//Q_EXPORT_PLUGIN2(image_viewer_factory, XMLEditorFactory)
Q_EXPORT_PLUGIN(XMLEditorPlugin)

