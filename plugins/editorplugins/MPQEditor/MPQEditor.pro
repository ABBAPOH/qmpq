TARGET = MPQEditor
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build

CONFIG += plugin \
    static
HEADERS += mpqeditor.h \
    mpqeditorplugin.h
SOURCES += mpqeditor.cpp \
    mpqeditorplugin.cpp
RESOURCES += mpqeditor.qrc
FORMS += 
