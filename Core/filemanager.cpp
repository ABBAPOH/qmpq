#include "filemanager.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

FileManager::FileManager(QObject *parent) :
    QObject(parent)
{
    m_extensionString = tr("All Files (*.*)");
}

QStringList FileManager::getOpenFileNames()
{

    QStringList result = QFileDialog::getOpenFileNames((QWidget*)0, tr("Select File"), "", m_extensionString);
    //        fileName = QFileDialog::getOpenFileName(this, tr("Select Archive File"));

    return result;
}

QString FileManager::getSaveFileName(const QString & dir, const QString & filter)
{
    QString result = QFileDialog::getSaveFileName((QWidget*)0, tr("Save As"), dir, (filter == "") ? m_extensionString : filter);
    return result;
}

int FileManager::confirmClose(const QString & file)
{
    QMessageBox messageBox;
    messageBox.setText("File " + file + " has been modified. Save changes?");
    messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Save);
    return messageBox.exec();
}

void FileManager::registerExtensionString(const QString & extensionString)
{
    m_registeredExtensionStrings.append(extensionString);
    m_extensionString = "";
    foreach (QString extension, m_registeredExtensionStrings) {
      m_extensionString.append(extension + ";;");
    }
    m_extensionString.append("All Files (*.*)");
//    m_extensionString.prepend(extensionString + ";;");
}
