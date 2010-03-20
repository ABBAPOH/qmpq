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
    Q_PROPERTY(QWidget * centralWidget READ centralWidget WRITE setCentralWidget NOTIFY centralWidgetChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString saveFilter READ saveFilter)

    QWidget * createWidgetForEditor(IEditor * editor);
    void cleanHistory(int start);
    bool openUrl(const QString & url);
    void setCentralWidget(QWidget * widget);

public:
    explicit EditorView(QWidget *parent = 0);
    ~EditorView();
    QString path() const { return m_path; }
    QWidget * centralWidget() const { return m_centralWidget; }
    QString saveFilter() const;

signals:
    void pathChanged(const QString & url);
    void centralWidgetChanged(QWidget * widget);

public slots:
    void back();
    void forward();
    void up();
    void setPath(const QString & url);
    void save(const QString & path = "");

private:
    QString m_path;
//    QHash<QString, IEditor *> m_editors;
    QHash<IEditor *, QWidget *> m_widgets;
    EditorManager * editorManager;
    QWidget * m_centralWidget;
    IEditor * m_editor;
    QStringList m_history;
    int historyPos;

};

#endif // EDITORVIEW_H
