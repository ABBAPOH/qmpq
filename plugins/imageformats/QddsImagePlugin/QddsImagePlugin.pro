TARGET = QddsImagePlugin
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build

HEADERS += dds.h \
    ddsheader.h \
    qdxt.h
SOURCES += dds.cpp \
    qdxt.cpp

unix: target.path = /usr/local/lib
mac: target.path = ../../../app/bin/QMPQ.app/Contents/MacOS/imageformats
win32: target.path = ../../../app/bin/imageformats
INSTALLS += target
