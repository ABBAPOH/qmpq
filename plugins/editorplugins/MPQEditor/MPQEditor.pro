TARGET = MPQEditor
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
CONFIG += plugin \
    static
INCLUDEPATH += -I \
    ../../../Core/
HEADERS += mpqeditor.h \
    mpqeditorplugin.h \
    universalview.h \
    idirmodel.h \
    compactprocessdialog.h
SOURCES += mpqeditor.cpp \
    mpqeditorplugin.cpp \
    main.cpp \
    universalview.cpp \
    idirmodel.cpp \
    compactprocessdialog.cpp
RESOURCES += mpqeditor.qrc
FORMS += compactprocessdialog.ui
