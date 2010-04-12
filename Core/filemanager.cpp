#include "filemanager.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

FileManager::FileManager(QObject *parent) :
    QObject(parent)
{
}

QStringList FileManager::getOpenFileNames()
{
    QStringList result = QFileDialog::getOpenFileNames();
    return result;
}

QString FileManager::getSaveFileName()
{
    QString result = QFileDialog::getSaveFileName();
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

