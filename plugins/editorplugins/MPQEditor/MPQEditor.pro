TARGET = MPQEditor
TEMPLATE = lib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
CONFIG += plugin \
    static
INCLUDEPATH += -I \
    ../../../Core/
HEADERS += mpqeditor.h \
    mpqeditorplugin.h \
    universalview.h \
    idirmodel.h \
    compactprocessdialog.h \
    hashtablesizedialog.h \
    powerspinbox.h \
    myfileiconprovider.h \
    verifyfilesdialog.h \
    changeattributesdialog.h \
    archivepropertiesdialog.h \
    filepropertiesdialog.h \
    setfilelocaledialog.h
SOURCES += mpqeditor.cpp \
    mpqeditorplugin.cpp \
    main.cpp \
    universalview.cpp \
    idirmodel.cpp \
    compactprocessdialog.cpp \
    hashtablesizedialog.cpp \
    powerspinbox.cpp \
    myfileiconprovider.cpp \
    verifyfilesdialog.cpp \
    changeattributesdialog.cpp \
    archivepropertiesdialog.cpp \
    filepropertiesdialog.cpp \
    setfilelocaledialog.cpp
RESOURCES += mpqeditor.qrc
FORMS += compactprocessdialog.ui \
    hashtablesizedialog.ui \
    verifyfilesdialog.ui \
    changeattributesdialog.ui \
    archivepropertiesdialog.ui \
    filepropertiesdialog.ui \
    setfilelocaledialog.ui
