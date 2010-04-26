TARGET = ImageViewer
TEMPLATE = lib
CONFIG += plugin

DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

INCLUDEPATH += -I ../../../core/
LIBS += -L../../../core/bin -lcore

HEADERS += imageviewer.h \
    imageviewerplugin.h
SOURCES += imageviewer.cpp \
    imageviewerplugin.cpp
FORMS += imageviewer.ui \
    imageSettingsDialog.ui
target.path += ../../../app/bin/plugins
RESOURCES += imageviewer.qrc

INSTALLS += target