#-------------------------------------------------
#
# Project created by QtCreator 2013-11-25T20:40:35
#
#-------------------------------------------------

TEMPLATE = app
include(../../Tsunami.pri)

CONFIG += gui

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
}

TRANSLATIONS += i18n/tsunami_en.ts \
                i18n/tsunami_ru.ts

LIBS += -L$$DESTDIR -lcore


SOURCES += main.cpp\
        devicewindow.cpp \
    opendevicedialog.cpp \
    librarywindow.cpp \
    openlibrarydialog.cpp \
    prepareextractordialog.cpp \
    extractorwindow.cpp

HEADERS  += devicewindow.h \
    opendevicedialog.h \
    librarywindow.h \
    openlibrarydialog.h \
    prepareextractordialog.h \
    extractorwindow.h

FORMS    += devicewindow.ui \
    opendevicedialog.ui \
    librarywindow.ui \
    openlibrarydialog.ui \
    prepareextractordialog.ui \
    extractorwindow.ui

RESOURCES += \
    resources.qrc
