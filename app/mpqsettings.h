#ifndef MPQSETTINGS_H
#define MPQSETTINGS_H

#include <QtGui/QWidget>
#include <QtCore/QMetaEnum>
#include <QtCore/QModelIndex>
#include <ipreferences.h>

namespace Ui {
    class MPQSettings;
}

class MPQExtensionManager;
class MPQSettings : public QWidget {
    Q_OBJECT
public:
    MPQSettings(QWidget *parent = 0);
    ~MPQSettings();
    MPQExtensionManager * extensionManager() { return m_extensionManager; }
    void addExtension(const QString & key, int value);

protected:
    void changeEvent(QEvent *e);

private:
//    QString compresionValueToKeys(int value);
//    int compresionValue(int index);
    int compresionValueToIndex(int value);
//    int keyToValue(QString flagsName, int value);

    Ui::MPQSettings *ui;
    MPQExtensionManager * m_extensionManager;
    QMetaEnum compressionEnum;

private slots:
    void addExtension();
    void removeExtension();
    void onClick(const QModelIndex &);
    void onEditText(const QString &);
    void onActivate(int);
};

class MPQSettingsPage : public IPreferencesPage
{
public:
    MPQSettingsPage(QObject * parent = 0) : IPreferencesPage(parent), m_widget(new MPQSettings)
    {
    }

    ~MPQSettingsPage() {
    }

    QStringList keys();

    void setDefaults();
    QVariant value(const QString & key);
    void setValue(const QString & key, const QVariant & value);

    virtual QString name() { return "Rules for adding file to MPQ"; }
    virtual QString key() { return "MPQ/rules"; }
    virtual QWidget * widget() { return m_widget; }

private:
    MPQSettings * m_widget;
};


#ifndef COMBOBOX_DELEGATE_H
#define COMBOBOX_DELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>

class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ComboBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
//    void paint(QPainter *painter, const QStyleOptionViewItem &option,
//                    const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // COMBOBOX__DELEHATE_H

#endif // MPQSETTINGS_H
