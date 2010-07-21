# -------------------------------------------------
# Project created by QtCreator 2009-10-10T12:53:39
# -------------------------------------------------
TARGET = Core
TEMPLATE = lib

unix:DESTDIR = ../bin
mac:DESTDIR = ../bin/QMPQ.app/Contents/MacOS
win32:DESTDIR = ../bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

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
    filemanager.h \
    ipreferences.h \
    preferencesmanager.h \
    coreconstants.h

SOURCES += core.cpp \
    pluginmanager.cpp \
    basecontext.cpp \
    editorfactorymanager.cpp \
    icore.cpp \
    actionmanager.cpp \
    filemanager.cpp \
    preferencesmanager.cpp
