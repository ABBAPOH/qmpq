TARGET = QtgaImagePlugin
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build

HEADERS += tga.h
SOURCES += tga.cpp

unix: target.path = /usr/local/lib
mac: target.path = ../../../app/bin/QMPQ.app/Contents/MacOS/imageformats
win32: target.path = ../../../app/bin/imageformats
INSTALLS += target
