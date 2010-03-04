#include "codeeditorplugin.h"

#include <QtCore/QtPlugin>
#include <QtGui/QToolBar>
#include <QtGui/QFileDialog>
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
#include <QDebug>

#include "codeeditor.h"

//================================== CodeEditorPluginPlugin ==================================

CodeEditorPlugin::CodeEditorPlugin(CodeEditor * editor)
{
    m_editor = editor;
    m_toolBar = new QToolBar();
}

CodeEditorPlugin::~CodeEditorPlugin()
{
}

bool CodeEditorPlugin::open(const QString &file)
{
    m_editor->open(file);
    return true;
}

void CodeEditorPlugin::close()
{
    m_editor->closeFile();
}

QWidget * CodeEditorPlugin::widget()
{
    return m_editor;
}

QToolBar * CodeEditorPlugin::toolBar ()
{
    return m_toolBar;
}

//================================== CodeEditorFactory ==================================

CodeEditorFactory::CodeEditorFactory()
{

}

IEditor * CodeEditorFactory::createEditor(QWidget * parent)
{
//    qDebug("CodeEditorFactory::instance");
    CodeEditor * editor = new CodeEditor(parent);
    return new CodeEditorPlugin(editor);
}

void CodeEditorFactory::shutdown()
{
//    qDebug("CodeEditorFactory::shutdown");
//    delete MPQEditor::model();
}

bool CodeEditorFactory::canHandle(const QString &file) const
{
//    qDebug("CodeEditorFactory::canHandle");
    QStringList extensions;
    QString suffix = QFileInfo(file).suffix().toLower();
    extensions << "txt" << "j" << "ai" << "wts" << "galaxy";
    foreach (QString extension, extensions)
        if (suffix == extension)
            return true;
    return false;
}

Q_EXPORT_PLUGIN2(image_viewer_factory, CodeEditorFactory)

