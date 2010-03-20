#ifndef QMPQIMAGEVIEWER_H
#define QMPQIMAGEVIEWER_H

#include <QPlainTextEdit>

class LineNumberArea;
class QSyntaxHighlighter;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    friend class LineNumberArea;
public:
    CodeEditor(QWidget *parent = 0);
    ~CodeEditor();
    void setHighligher(const QString & suffix);

    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event);
    void lineNumberAreaPaintEvent(QPaintEvent *event);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    LineNumberArea *lineNumberArea;
    QSyntaxHighlighter * highlighter;
};

#endif // QMPQIMAGEVIEWER_H
