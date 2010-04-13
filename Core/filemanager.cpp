#include "filemanager.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#warning TODO: fix lock!!!!
#include "../QMPQFileEngine/qmpqfileenginehandler.h"

FileManager::FileManager(QObject *parent) :
    QObject(parent)
{
}

QStringList FileManager::getOpenFileNames()
{

    QMPQFileEngineHandler::setLocked(true);
    QStringList result = QFileDialog::getOpenFileNames();
    //        fileName = QFileDialog::getOpenFileName(this, tr("Select Archive File"));
    QMPQFileEngineHandler::setLocked(false);

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

