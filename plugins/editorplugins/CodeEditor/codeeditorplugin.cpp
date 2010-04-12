#include "codeeditorplugin.h"

#include <QtCore/QtPlugin>
#include <QtGui/QToolBar>
#include <QtGui/QFileDialog>
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
#include <QDebug>

#include "codeeditor.h"

//================================== CodeEditorPluginPlugin ==================================

CodeEditorInterface::CodeEditorInterface(CodeEditor * editor)
{
    m_editor = editor;
    m_toolBar = new QToolBar();
    initToolBar();

    connect(m_editor, SIGNAL(modificationChanged(bool)), SIGNAL(changed()));
}

void CodeEditorInterface::initToolBar()
{
    m_toolBar->addAction(QIcon(":/icons/images/save.png"), "save", m_editor, SLOT(save()));
    m_toolBar->addSeparator();
    m_toolBar->addAction(QIcon(":/icons/images/cut.png"), "cut", m_editor, SLOT(cut()));
    m_toolBar->addAction(QIcon(":/icons/images/copy.png"), "copy", m_editor, SLOT(copy()));
    m_toolBar->addAction(QIcon(":/icons/images/paste.png"), "paste", m_editor, SLOT(paste()));
    m_toolBar->addSeparator();
    m_toolBar->addAction(QIcon(":/icons/images/undo.png"), "undo", m_editor, SLOT(undo()));
    m_toolBar->addAction(QIcon(":/icons/images/redo.png"), "redo", m_editor, SLOT(redo()));
}

CodeEditorInterface::~CodeEditorInterface()
{
}

bool CodeEditorInterface::isModified()
{
    return m_editor->isModified();
}

bool CodeEditorInterface::open(const QString &filePath)
{
    if (!filePath.isEmpty()) {
        m_currentFile = filePath;
        QFile file(filePath);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            m_editor->setPlainText(file.readAll());
            m_editor->setHighligher(QFileInfo(filePath).suffix());
        }
    }
    
//    m_editor->open(filePath);
    return true;
}

void CodeEditorInterface::save(const QString &filePath)
{
    QFile file(filePath == "" ? m_currentFile : filePath);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
//        QString data = ui->textEdit->toPlainText();
        file.write(m_editor->toPlainText().toLocal8Bit());
        file.close();
        m_editor->setModified(false);
    }
}

void CodeEditorInterface::close()
{
    m_editor->clear();
}

QWidget * CodeEditorInterface::widget()
{
    return m_editor;
}

QToolBar * CodeEditorInterface::toolBar()
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
    return new CodeEditorInterface(editor);
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

//================================== CodeEditorPlugin ==================================

void CodeEditorPlugin::initialize()
{
    ICore::instance()->editorFactoryManager()->addFactory(new CodeEditorFactory);
}

//Q_EXPORT_PLUGIN2(image_viewer_factory, CodeEditorFactory)
Q_EXPORT_PLUGIN(CodeEditorPlugin)

