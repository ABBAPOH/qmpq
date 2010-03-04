#include "galaxysyntaxhighlighter.h"

#include <QSettings>

GalaxySyntaxHighlighter::GalaxySyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    QSettings s("galaxy.ini", QSettings::IniFormat);
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::black);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywords = s.value("keywords").toStringList();

    foreach (const QString &pattern, keywords) {
        rule.pattern = QRegExp("\\b" + pattern + "\\b");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    QStringList typePatterns = s.value("types").toStringList();
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Normal);

             ;
    foreach (const QString &pattern, typePatterns) {
        rule.pattern = QRegExp("\\b" + pattern + "\\b");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    constFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegExp("\\b[0-9]\\b");
    rule.format = constFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegExp("\\bnull|true|false\\b");
    rule.format = constFormat;
    highlightingRules.append(rule);

    stringFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
}

void GalaxySyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
}
