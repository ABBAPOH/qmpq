TARGET = ImageViewer
TEMPLATE = lib
CONFIG += plugin

DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

INCLUDEPATH += -I \
    ../../../Core

LIBS += -L../../../Core/bin \
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

target.path += ../../../app/bin/plugins
mac:target.path = ../../../app/bin/QMPQ.app/Contents/MacOS/qmpqplugins
INSTALLS += target
