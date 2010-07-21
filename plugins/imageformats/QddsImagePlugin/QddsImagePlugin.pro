TARGET = QddsImagePlugin
TEMPLATE = lib
unix:DESTDIR = ../../../bin/imageformats
mac:DESTDIR = ../../../bin/QMPQ.app/Contents/MacOS/imageformats
win32:DESTDIR = ../../../bin/imageformats
OBJECTS_DIR = build
MOC_DIR = build

HEADERS += dds.h \
    ddsheader.h \
    qdxt.h
SOURCES += dds.cpp \
    qdxt.cpp
