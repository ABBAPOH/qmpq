TARGET = XMLViewer
TEMPLATE = lib
CONFIG += plugin
QT += xml

DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

INCLUDEPATH += -I \
    ../../../Core

LIBS += -L../../../Core/bin \
    -lCore

INCLUDEPATH += QXMLViewer/

DEPENDPATH +=  QXMLViewer/
include(QXMLViewer/QXMLViewer.pro)
#INCLUDEPATH += /Users/arch/c++/Qt4/QXMLViewer/

HEADERS += xmleditorplugin.h \
#    xmleditor.h \

SOURCES += xmleditorplugin.cpp \
#    xmleditor.cpp \

target.path += ../../../app/bin/plugins
INSTALLS += target
