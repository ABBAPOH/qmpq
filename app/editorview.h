#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QtGui/QStackedWidget>
#include <QtCore/QHash>
#include <QtCore/QString>

class IEditor;
class EditorManager;
class EditorView : public QStackedWidget
{
Q_OBJECT
    QString m_current;
    QHash<QString, IEditor *> m_editors;
    QHash<IEditor *, QWidget *> m_widgets;
    EditorManager * editorManager;

    QWidget * createWidgetForEditor(IEditor * editor);
    bool openUrl(const QString & url);

public:
    explicit EditorView(QWidget *parent = 0);
    ~EditorView();
    QString currentUrl() { return m_current; }

signals:
    void currentUrlChanged(const QString & url);

public slots:
    void back();
    void forward();
    void up();
    void setUrl(const QString & url);
};

#endif // EDITORVIEW_H
