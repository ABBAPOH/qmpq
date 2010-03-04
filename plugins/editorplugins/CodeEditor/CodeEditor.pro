TARGET = ImageViewer
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
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
FORMS += 
target.path += ../../../app/bin/plugins
INSTALLS += target
