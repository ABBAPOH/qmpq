# -------------------------------------------------
# Project created by QtCreator 2009-10-10T12:53:39
# -------------------------------------------------
TARGET = QMPQ
TEMPLATE = app

DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

LIBS += -L../plugins/editorplugins/MPQEditor/bin/ \
    -lMPQEditor
LIBS += -L../QMPQFileEngine/bin \
    -lQMPQFileEngine
LIBS += -L../3rdParty/FileSystemModel/bin \
    -lFileSystemModel
LIBS += -L../3rdParty/QDirModel/bin \
    -lQDirModel
LIBS += -L../Core/bin/ \
    -lCore

INCLUDEPATH += -I \
    ../QMPQFileEngine
INCLUDEPATH += -I \
    ../Core

HEADERS += mainwindow.h \
    editormanager.h \
    editorview.h \
    tabwidget.h \
    windowmanager.h \
    tabcontext.h \
    tabmanager.h \
    mainwindowplugin.h \
    historymanager.h \
    createarchivedialog.h \
    archivesuffixesmanager.h
SOURCES += mainwindow.cpp \
    main.cpp \
    editormanager.cpp \
    editorview.cpp \
    tabwidget.cpp \
    windowmanager.cpp \
    tabcontext.cpp \
    tabmanager.cpp \
    mainwindowplugin.cpp \
    historymanager.cpp
FORMS += createarchivedialog.ui
RESOURCES += qmq.qrc
