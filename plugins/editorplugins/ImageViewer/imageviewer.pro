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
    imageviewerplugin.h
SOURCES += imageviewer.cpp \
    imageviewerplugin.cpp
FORMS += imageviewer.ui \
    imageSettingsDialog.ui

RESOURCES += imageviewer.qrc

target.path += ../../../app/bin/plugins
INSTALLS += target

