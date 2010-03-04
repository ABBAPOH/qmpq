#include "qmpqcodeeditor.h"
#include "ui_codeeditor.h"
#include "jasssyntaxhighlighter.h"

#include <QFileDialog>

QMPQCodeEditor::QMPQCodeEditor(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CodeEditor)
{
    ui->setupUi(this);
    setWindowTitle(tr("QMPQ Code Editor"));
//    highlighter = new JassSyntaxHighlighter(ui->textEdit->document());
    connect(ui->actionSave, SIGNAL(triggered()), SLOT(save()));
}

QMPQCodeEditor::~QMPQCodeEditor()
{

}

void QMPQCodeEditor::openFile(const QString & path)
{
     QString fileName = path;

     if (fileName.isNull())
         fileName = QFileDialog::getOpenFileName(this,
             tr("Open File"), "", "C++ Files (*.cpp *.h)");

     if (!fileName.isEmpty()) {
         currentFile = fileName;
         QFile file(fileName);
         if (file.open(QFile::ReadOnly | QFile::Text))
             ui->textEdit->setPlainText(file.readAll());
     }
}

void QMPQCodeEditor::save()
{
    QFile file(currentFile);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
//        QString data = ui->textEdit->toPlainText();
        file.write(ui->textEdit->toPlainText().toLocal8Bit());
        file.close();
        emit fileSaved(currentFile);
    }
}

