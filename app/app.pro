# -------------------------------------------------
# Project created by QtCreator 2009-10-10T12:53:39
# -------------------------------------------------
TARGET = QMPQ
TEMPLATE = app
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build

LIBS += -L../QMPQFileEngine/bin \
    -lQMPQFileEngine
LIBS += -L../plugins/editorplugins/MPQEditor/bin/ \
    -lMPQEditor

RESOURCES += qmq.qrc

INCLUDEPATH += -I \
    ../QMPQFileEngine

HEADERS += mainwindow.h \
    ieditor.h \
    ieditorfactory.h \
    pluginmanager.h \
    editormanager.h \
    editorview.h
SOURCES += mainwindow.cpp \
    main.cpp \
    pluginmanager.cpp \
    editormanager.cpp \
    editorview.cpp
FORMS += mainwindow.ui
