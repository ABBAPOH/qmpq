# -------------------------------------------------
# Project created by QtCreator 2009-10-10T12:53:39
# -------------------------------------------------
TARGET = QMPQ
TEMPLATE = app
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
LIBS += -L../QMPQFileEngine/bin \
    -lQMPQFileEngine
LIBS += -L../plugins/editorplugins/MPQEditor/bin/ \
    -lMPQEditor
LIBS += -L../Core/bin/ \
    -lCore
RESOURCES += qmq.qrc
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
    historymanager.h
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
