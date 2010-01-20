#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include "mpqeditor.h"
#include <QDebug>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    MPQEditor w;
//    w.show();
    w.showMaximized();
    w.open("/Users/arch");
    w.setViewMode(MPQEditor::ColumnView);
    QTimer tim;
    tim.singleShot(10000, &w, SLOT(newFolder()));
//    w.newFolder();

    return app.exec();
}
