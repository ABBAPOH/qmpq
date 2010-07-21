# -------------------------------------------------
# Project created by QtCreator 2009-11-19T09:03:48
# -------------------------------------------------
QT -= gui
TARGET = QMPQFileEngine
TEMPLATE = lib

mac: {
    CONFIG += lib_bundle
}
#TEMPLATE = app

unix:DESTDIR = ../bin/
mac:DESTDIR = ../bin/QMPQ.app/Contents/MacOS
win32:DESTDIR = ../bin/

OBJECTS_DIR = build
MOC_DIR = build

INCLUDEPATH += ../3rdParty
LIBS += -lStormLib
mac:LIBS = -framework StormLib

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
