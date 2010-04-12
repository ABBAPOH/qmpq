#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>

class IEditor;
class QVBoxLayout;
class EditorView : public QWidget
{
Q_OBJECT
public:
    explicit EditorView(QWidget *parent = 0);

    void setEditor(IEditor * editor);
    IEditor * editor() { return m_editor; }

signals:

public slots:

private:
    IEditor * m_editor;
    QVBoxLayout * layout;
};

#endif // EDITORVIEW_H
