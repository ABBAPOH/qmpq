# -------------------------------------------------
# Project created by QtCreator 2009-11-19T09:03:48
# -------------------------------------------------
QT -= gui
TARGET = QMPQFileEngine
TEMPLATE = lib

# TEMPLATE = app
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
INCLUDEPATH += ../3rdParty
LIBS += -lStorm
DEFINES += QMPQFILEENGINE_LIBRARY
SOURCES += qmpqfileengine.cpp \
    main.cpp \
    qmpqfileenginehandler.cpp \
    mpqfileengineiterator.cpp \
    treeitem.cpp \
    qmpqerror.cpp \
    qmpqarchive.cpp \
    sharedmpqarchive.cpp \
    objectcache.cpp \
    qmpqarchivecache.cpp \
    mpqsettings.cpp
HEADERS += qmpqfileengine.h \
    QMPQFileEngine_global.h \
    qmpqfileenginehandler.h \
    mpqfileengineiterator.h \
    treeitem.h \
    qmpqerror.h \
    qmpqarchive.h \
    qmpqarchive_p.h \
    sharedmpqarchive.h \
    qmpqfileengine_p.h \
    objectcache.h \
    qmpqarchivecache.h \
    mpqsettings.h
mac:target.path += ../app/bin/QMPQ.app/Contents/MacOS
win32:target.path += ../app/bin/
INSTALLS += target
