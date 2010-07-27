TARGET = XMLViewer
TEMPLATE = lib
CONFIG += plugin
QT += xml

include(QXMLViewer/QXMLViewer.pro)

DESTDIR = ../../../bin/plugins
mac:DESTDIR = ../../../bin/QMPQ.app/Contents/MacOS/qmpqplugins
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

INCLUDEPATH += -I \
    ../../../Core

LIBS += -L../../../bin \
    -lCore

INCLUDEPATH += QXMLViewer/

DEPENDPATH +=  QXMLViewer/
#INCLUDEPATH += /Users/arch/c++/Qt4/QXMLViewer/

HEADERS += xmleditorplugin.h \
#    xmleditor.h \

SOURCES += xmleditorplugin.cpp \
#    xmleditor.cpp \
