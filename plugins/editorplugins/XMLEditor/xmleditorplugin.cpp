#include "xmleditorplugin.h"

#include <QtCore/QFileInfo>
#include <QtCore/QtPlugin>
#include <QtGui/QToolBar>

#include <qxmlviewer.h>

//================================== XMLEditorPluginPlugin ==================================

XMLEditorPlugin::XMLEditorPlugin(QXMLViewer * editor)
{
    m_editor = editor;
    m_toolBar = new QToolBar();

//    initToolBar();
}

XMLEditorPlugin::~XMLEditorPlugin()
{
}

bool XMLEditorPlugin::open(const QString &filePath)
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

void XMLEditorPlugin::close()
{
}

QWidget * XMLEditorPlugin::widget()
{
    return m_editor;
}

QToolBar * XMLEditorPlugin::toolBar ()
{
    return m_toolBar;
}

//================================== XMLEditorFactory ==================================

XMLEditorFactory::XMLEditorFactory()
{

}

IEditor * XMLEditorFactory::createEditor(QWidget * parent)
{
//    qDebug("XMLEditorFactory::instance");
    QXMLViewer * editor = new QXMLViewer(parent);
    return new XMLEditorPlugin(editor);
}

void XMLEditorFactory::shutdown()
{
//    qDebug("XMLEditorFactory::shutdown");
//    delete MPQEditor::model();
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

Q_EXPORT_PLUGIN2(image_viewer_factory, XMLEditorFactory)

