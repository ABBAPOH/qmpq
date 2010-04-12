#ifndef EDITOR_MANAGER_H
#define EDITOR_MANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>

class IEditor;
class IEditorFactory;
class EditorManager : public QObject
{
Q_OBJECT
public:
    explicit EditorManager(QObject *parent = 0);
    IEditor * open(const QString &file);
    bool close(const QString &file);
    bool isOpened(const QString &file) { return m_editors.contains(file); }
    bool isOpened(IEditor *editor) { return m_editors.values().contains(editor); }
    IEditor * editor(const QString &file);
//    int count(IEditor *editor) { return m_editors.values().count(editor); }

signals:
    void openRequested(const QString &file);

public slots:

private:
    QHash<QString, IEditor*> m_editors;
};

#endif // EDITOR_MANAGER_H
