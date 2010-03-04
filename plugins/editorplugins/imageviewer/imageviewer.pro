TARGET = ImageViewer
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
CONFIG += plugin
HEADERS += imageviewer.h \
    imageviewerplugin.h
SOURCES += imageviewer.cpp \
    imageviewerplugin.cpp
FORMS += imageviewer.ui \
    imageSettingsDialog.ui
target.path += ../../../app/bin/plugins
INSTALLS += target
