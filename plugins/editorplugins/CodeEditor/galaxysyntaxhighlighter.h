#ifndef GALAXYSYNTAXHIGHLIGHTER_H
#define GALAXYSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

//#include <QHash>
#include <QTextCharFormat>

class QTextDocument;

class GalaxySyntaxHighlighter : public QSyntaxHighlighter
{
public:
    GalaxySyntaxHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat constFormat;

    QTextCharFormat stringFormat;
    QTextCharFormat functionFormat;
};

#endif // GALAXYSYNTAXHIGHLIGHTER_H
