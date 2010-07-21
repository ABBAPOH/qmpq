TARGET = QblpImagePlugin

TEMPLATE = lib
unix:DESTDIR = ../../../bin/imageformats
mac:DESTDIR = ../../../bin/QMPQ.app/Contents/MacOS/imageformats
win32:DESTDIR = ../../../bin/imageformats
OBJECTS_DIR = build
MOC_DIR = build

INCLUDEPATH += -I \
    ../../../3rdparty/libjpeg/
LIBS += -L../../../3rdparty/libjpeg/bin -ljpeg

HEADERS += blp.h \
    jpeg/qjpeghandler.h \
    blpheader.h \
    octree.h
SOURCES += blp.cpp \
    jpeg/qjpeghandler.cpp \
    blp2.cpp \
    main.cpp \
    blpheader.cpp \
    octree.cpp
