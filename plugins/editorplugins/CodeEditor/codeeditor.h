#ifndef QMPQIMAGEVIEWER_H
#define QMPQIMAGEVIEWER_H

#include <QPlainTextEdit>
//#include <QSettings>

//namespace Ui
//{
//    class CodeEditor;
//}

class LineNumberArea;
class QSyntaxHighlighter;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    friend class LineNumberArea;
public:
    CodeEditor(QWidget *parent = 0);
    ~CodeEditor();

    int lineNumberAreaWidth();

public slots:
    void open(const QString & path = QString());
    void closeFile();
    void save(const QString & path = "");

protected:
    void resizeEvent(QResizeEvent *event);
    void lineNumberAreaPaintEvent(QPaintEvent *event);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    void setHighligher();
//    Ui::CodeEditor *ui;
    LineNumberArea *lineNumberArea;
    QString currentFile;
    QSyntaxHighlighter * highlighter;
};

#endif // QMPQIMAGEVIEWER_H
