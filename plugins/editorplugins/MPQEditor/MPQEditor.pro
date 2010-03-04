TARGET = MPQEditor
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
CONFIG += plugin \
    static
HEADERS += mpqeditor.h \
    mpqeditorplugin.h
SOURCES += mpqeditor.cpp \
    mpqeditorplugin.cpp \
    main.cpp
RESOURCES += mpqeditor.qrc
FORMS += 
