TARGET = ImageViewer
TEMPLATE = lib
CONFIG += plugin

DESTDIR += ../../../bin/qmpqplugins
mac:DESTDIR = ../../../bin/QMPQ.app/Contents/MacOS/qmpqplugins
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

INCLUDEPATH += -I \
    ../../../Core

LIBS += -L../../../bin \
    -lCore

HEADERS += imageviewer.h \
    imageviewerplugin.h \
    preferences.h
SOURCES += imageviewer.cpp \
    imageviewerplugin.cpp \
    preferences.cpp
FORMS += imageviewer.ui \
    imageSettings.ui

RESOURCES += imageviewer.qrc
