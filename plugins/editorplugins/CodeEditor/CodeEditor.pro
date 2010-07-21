TARGET = CodeEditor
TEMPLATE = lib

DESTDIR += ../../../bin/plugins
mac:DESTDIR = ../../../bin/QMPQ.app/Contents/MacOS/qmpqplugins

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

CONFIG += plugin

INCLUDEPATH += -I \
    ../../../Core

LIBS += -L../../../Core/bin \
    -lCore

HEADERS += codeeditor.h \
    codeeditorplugin.h \
    linenumberarea.h \
    jasssyntaxhighlighter.h \
    galaxysyntaxhighlighter.h

SOURCES += codeeditor.cpp \
    codeeditorplugin.cpp \
    jasssyntaxhighlighter.cpp \
    galaxysyntaxhighlighter.cpp

RESOURCES += codeeditor.qrc
