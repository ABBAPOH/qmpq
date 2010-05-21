TARGET = QblpImagePlugin

TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build

INCLUDEPATH += -I \
    ../../../3rdparty/libjpeg/
LIBS += -L../../../3rdparty/libjpeg/bin -ljpeg

HEADERS += blp.h \
    jpeg/qjpeghandler.h \
    blpheader.h
SOURCES += blp.cpp \
    jpeg/qjpeghandler.cpp \
    blp2.cpp \
    main.cpp \
    blpheader.cpp

unix:target.path = /usr/local/lib
mac:target.path = ../../../app/bin/QMPQ.app/Contents/MacOS/imageformats
win32:target.path = ../../../app/bin/imageformats
INSTALLS += target

