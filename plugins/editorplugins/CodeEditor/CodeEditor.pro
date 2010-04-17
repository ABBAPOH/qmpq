TARGET = CodeEditor
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
CONFIG += plugin
INCLUDEPATH += -I \
    ../../../core/
LIBS += -L../../../core/bin \
    -lcore
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
target.path += ../../../app/bin/plugins
INSTALLS += target
