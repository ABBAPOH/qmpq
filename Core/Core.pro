# -------------------------------------------------
# Project created by QtCreator 2009-10-10T12:53:39
# -------------------------------------------------
TARGET = Core

TEMPLATE = lib

DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

LIBS += -L../QMPQFileEngine/bin \
    -lQMPQFileEngine

HEADERS += icore.h \
    icontext.h \
    ipluginmanager.h \
    iwindowmanager.h \
    ieditorfactorymanager.h \
    iplugin.h \
    core.h \
    pluginmanager.h \
    basecontext.h \
    editorfactorymanager.h \
    ieditorfactory.h \
    ieditor.h \
    iactionmanager.h \
    actionmanager.h \
    filemanager.h

SOURCES += core.cpp \
    pluginmanager.cpp \
    basecontext.cpp \
    editorfactorymanager.cpp \
    icore.cpp \
    actionmanager.cpp \
    filemanager.cpp

mac:target.path += ../app/bin/QMPQ.app/Contents/MacOS
win32:target.path += ../app/bin
INSTALLS += target
