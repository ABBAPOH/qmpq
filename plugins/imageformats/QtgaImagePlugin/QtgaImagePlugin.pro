TARGET = QtgaImagePlugin
TEMPLATE = lib
unix:DESTDIR = ../../../bin/imageformats
mac:DESTDIR = ../../../bin/QMPQ.app/Contents/MacOS/imageformats
win32:DESTDIR = ../../../bin/imageformats
OBJECTS_DIR = build
MOC_DIR = build

HEADERS += tga.h
SOURCES += tga.cpp
