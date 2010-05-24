# -------------------------------------------------
# Project created by QtCreator 2009-11-19T09:03:48
# -------------------------------------------------
#QT -= gui
TARGET = test
TEMPLATE = app
CONFIG -= app_bundle
CONFIG += console

DESTDIR = ../bin
OBJECTS_DIR = build
MOC_DIR = build

INCLUDEPATH += ../
LIBS += -L../bin/ \
    -lQMPQFileEngine

DEPENDPATH = ../

SOURCES += main.cpp \
    test.cpp
HEADERS += test.h

