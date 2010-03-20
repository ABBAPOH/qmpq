#ifndef QMPQIMAGEVIEWER_H
#define QMPQIMAGEVIEWER_H

#include <QPlainTextEdit>

class LineNumberArea;
class QSyntaxHighlighter;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    Q_PROPERTY(bool modified READ isModified WRITE setModified)
    friend class LineNumberArea;

public:
    CodeEditor(QWidget *parent = 0);
    ~CodeEditor();
    void setHighligher(const QString & suffix);

    int lineNumberAreaWidth();
    bool isModified() { return document()->isModified(); }

protected:
    void resizeEvent(QResizeEvent *event);
    void lineNumberAreaPaintEvent(QPaintEvent *event);

public slots:
    void setModified(bool modified) { document()->setModified(modified); }

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    LineNumberArea *lineNumberArea;
    QSyntaxHighlighter * highlighter;
};

#endif // QMPQIMAGEVIEWER_H
