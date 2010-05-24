# -------------------------------------------------
# Project created by QtCreator 2009-11-19T09:03:48
# -------------------------------------------------
#QT -= gui
TARGET = QMPQFileEngine
TEMPLATE = lib

#TEMPLATE = app

DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build

INCLUDEPATH += ../3rdParty
LIBS += -lStorm

DEFINES += QMPQFILEENGINE_LIBRARY

SOURCES += qmpqfileengine.cpp \
    qmpqfileenginehandler.cpp \
    mpqfileengineiterator.cpp \
    objectcache.cpp \
    qmpqarchivecache.cpp \
    mpqsettings.cpp \
    qmpqarchive.cpp \
    mpqfileinfo.cpp \
    mpqfileinfoiterator.cpp \
    qmpqarchiveex.cpp

HEADERS += qmpqfileengine.h \
    QMPQFileEngine_global.h \
    qmpqfileenginehandler.h \
    mpqfileengineiterator.h \
    qmpqarchive_p.h \
    qmpqfileengine_p.h \
    objectcache.h \
    qmpqarchivecache.h \
    mpqsettings.h \
    qmpqarchive.h \
    mpqfileinfo.h \
    qmpqarchive_p.h \
    mpqfileinfoiterator.h \
    qmpqarchiveex.h
unix:target.path = /usr/local/lib
mac:target.path = ../app/bin/QMPQ.app/Contents/MacOS
win32:target.path = ../app/bin/
INSTALLS += target
