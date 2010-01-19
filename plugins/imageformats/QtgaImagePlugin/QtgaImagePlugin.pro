TARGET = QtgaImagePlugin
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build

HEADERS += tga.h
SOURCES += tga.cpp

mac: target.path += ../../../app/bin/QMPQ.app/Contents/MacOS/imageformats
INSTALLS += target
