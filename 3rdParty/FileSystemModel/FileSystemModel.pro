TEMPLATE = lib
TARGET = FileSystemModel
CONFIG += static

DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

DEFINES += FILESYSTEMMODEL_LIBRARY

HEADERS += filesystemmodel.h \
    private/qfilesystemmodel_p.h

SOURCES += filesystemmodel.cpp
