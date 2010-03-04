TARGET = CodeEditor
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
CONFIG += plugin
HEADERS += codeeditor.h \
    codeeditorplugin.h \
    linenumberarea.h \
    jasssyntaxhighlighter.h \
    galaxysyntaxhighlighter.h
SOURCES += codeeditor.cpp \
    codeeditorplugin.cpp \
    jasssyntaxhighlighter.cpp \
    galaxysyntaxhighlighter.cpp
target.path += ../../../app/bin/plugins
INSTALLS += target
RESOURCES += codeeditor.qrc
