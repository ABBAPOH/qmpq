#ifndef QMPQCODEEDITOR_H
#define QMPQCODEEDITOR_H

#include <QMainWindow>

namespace Ui
{
    class CodeEditor;
}

class QSyntaxHighlighter;

class QMPQCodeEditor : public QMainWindow
{
    Q_OBJECT
public:
    QMPQCodeEditor(QWidget *parent = 0);
    ~QMPQCodeEditor();
public slots:
    void openFile(const QString & path = QString());
private slots:
    void save();
private:
    Ui::CodeEditor *ui;
    QString currentFile;
    QSyntaxHighlighter * highlighter;
signals:
    void fileSaved(const QString &);
};

#endif // QMPQCODEEDITOR_H
