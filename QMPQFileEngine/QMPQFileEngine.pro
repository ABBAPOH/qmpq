# -------------------------------------------------
# Project created by QtCreator 2009-11-19T09:03:48
# -------------------------------------------------
QT -= gui
TARGET = QMPQFileEngine
TEMPLATE = lib
#TEMPLATE = app
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build

LIBS += -lStorm

DEFINES += QMPQFILEENGINE_LIBRARY
SOURCES += qmpqfileengine.cpp \
    main.cpp \
    qmpqfileenginehandler.cpp \
    mpqfileengineiterator.cpp \
    treeitem.cpp \
    qmpqerror.cpp \
    qmpqarchive.cpp \
    sharedmpqarchive.cpp
HEADERS += qmpqfileengine.h \
    QMPQFileEngine_global.h \
    qmpqfileenginehandler.h \
    mpqfileengineiterator.h \
    treeitem.h \
    qmpqerror.h \
    qmpqarchive.h \
    qmpqarchive_p.h \
    sharedmpqarchive.h \
    qmpqfileengine_p.h
