TARGET = QblpImagePlugin
TEMPLATE = lib
DESTDIR = bin
HEADERS += blp.h \
    jpeg/qjpeghandler.h \
    blpheader.h
SOURCES += blp.cpp \
    jpeg/qjpeghandler.cpp \
    blp2.cpp \
    main.cpp

# DEPENDPATH += jpeg
# include(jpeg/jpeg.pri)
# INCLUDEPATH += jpeg
wince*: { 
    DEFINES += NO_GETENV
    contains(CE_ARCH,x86):CONFIG -= stl \
        exceptions
    contains(CE_ARCH,x86):CONFIG += exceptions_off
}
contains(QT_CONFIG, system-jpeg) { 
    unix:LIBS += -ljpeg
    win32:LIBS += libjpeg.lib
}
!contains(QT_CONFIG, system-jpeg) { 
    INCLUDEPATH += -I \
        ../../../3rdparty/libjpeg/
    HEADERS += ../../../3rdparty/libjpeg/jpeglib.h
    SOURCES += ../../../3rdparty/libjpeg/jcapimin.c \
        ../../../3rdparty/libjpeg/jcapistd.c \
        ../../../3rdparty/libjpeg/jccoefct.c \
        ../../../3rdparty/libjpeg/jccolor.c \
        ../../../3rdparty/libjpeg/jcdctmgr.c \
        ../../../3rdparty/libjpeg/jchuff.c \
        ../../../3rdparty/libjpeg/jcinit.c \
        ../../../3rdparty/libjpeg/jcmainct.c \
        ../../../3rdparty/libjpeg/jcmarker.c \
        ../../../3rdparty/libjpeg/jcmaster.c \
        ../../../3rdparty/libjpeg/jcomapi.c \
        ../../../3rdparty/libjpeg/jcparam.c \
        ../../../3rdparty/libjpeg/jcphuff.c \
        ../../../3rdparty/libjpeg/jcprepct.c \
        ../../../3rdparty/libjpeg/jcsample.c \
        ../../../3rdparty/libjpeg/jctrans.c \
        ../../../3rdparty/libjpeg/jdapimin.c \
        ../../../3rdparty/libjpeg/jdapistd.c \
        ../../../3rdparty/libjpeg/jdatadst.c \
        ../../../3rdparty/libjpeg/jdatasrc.c \
        ../../../3rdparty/libjpeg/jdcoefct.c \
        ../../../3rdparty/libjpeg/jdcolor.c \
        ../../../3rdparty/libjpeg/jddctmgr.c \
        ../../../3rdparty/libjpeg/jdhuff.c \
        ../../../3rdparty/libjpeg/jdinput.c \
        ../../../3rdparty/libjpeg/jdmainct.c \
        ../../../3rdparty/libjpeg/jdmarker.c \
        ../../../3rdparty/libjpeg/jdmaster.c \
        ../../../3rdparty/libjpeg/jdmerge.c \
        ../../../3rdparty/libjpeg/jdphuff.c \
        ../../../3rdparty/libjpeg/jdpostct.c \
        ../../../3rdparty/libjpeg/jdsample.c \
        ../../../3rdparty/libjpeg/jdtrans.c \
        ../../../3rdparty/libjpeg/jerror.c \
        ../../../3rdparty/libjpeg/jfdctflt.c \
        ../../../3rdparty/libjpeg/jfdctfst.c \
        ../../../3rdparty/libjpeg/jfdctint.c \
        ../../../3rdparty/libjpeg/jidctflt.c \
        ../../../3rdparty/libjpeg/jidctfst.c \
        ../../../3rdparty/libjpeg/jidctint.c \
        ../../../3rdparty/libjpeg/jidctred.c \
        ../../../3rdparty/libjpeg/jmemmgr.c \
        ../../../3rdparty/libjpeg/jquant1.c \
        ../../../3rdparty/libjpeg/jquant2.c \
        ../../../3rdparty/libjpeg/jutils.c \
        ../../../3rdparty/libjpeg/jmemnobs.c
}
FORMS += 
