#ifndef CORE_H
#define CORE_H

#include <QtCore/QObject>
#include <QtCore/QHash>

class PluginManager;
class IEditor;
class IEditorFactory;
class EditorManager : public QObject
{
Q_OBJECT
    const PluginManager * m_pluginManager;
    QHash<QString, IEditor*> m_editors;
//    QHash<IEditor*, IEditorFactory*> m_factories;

public:
    explicit EditorManager(QObject *parent = 0);
    const PluginManager * pluginManager() const { return m_pluginManager; }
    IEditor * open(const QString &file);
    void close(const QString &file);
    bool isOpened(const QString &file) { return m_editors.contains(file); }
    IEditor * editor(const QString &file);
//    int count(IEditor *editor) { return m_editors.values().count(editor); }
    bool isOpened(IEditor *editor) { return m_editors.values().contains(editor); }

signals:
    void openRequested(const QString &file);

public slots:

};

#endif // CORE_H
