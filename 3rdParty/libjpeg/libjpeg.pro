TARGET = jpeg

TEMPLATE = lib
CONFIG += static
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build

HEADERS += jpeglib.h
SOURCES += jcapimin.c \
        jcapistd.c \
        jccoefct.c \
        jccolor.c \
        jcdctmgr.c \
        jchuff.c \
        jcinit.c \
        jcmainct.c \
        jcmarker.c \
        jcmaster.c \
        jcomapi.c \
        jcparam.c \
        jcphuff.c \
        jcprepct.c \
        jcsample.c \
        jctrans.c \
        jdapimin.c \
        jdapistd.c \
        jdatadst.c \
        jdatasrc.c \
        jdcoefct.c \
        jdcolor.c \
        jddctmgr.c \
        jdhuff.c \
        jdinput.c \
        jdmainct.c \
        jdmarker.c \
        jdmaster.c \
        jdmerge.c \
        jdphuff.c \
        jdpostct.c \
        jdsample.c \
        jdtrans.c \
        jerror.c \
        jfdctflt.c \
        jfdctfst.c \
        jfdctint.c \
        jidctflt.c \
        jidctfst.c \
        jidctint.c \
        jidctred.c \
        jmemmgr.c \
        jquant1.c \
        jquant2.c \
        jutils.c \
        jmemnobs.c
