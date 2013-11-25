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

LIBS += -L$$DESTDIR -lcore


SOURCES += main.cpp\
        devicewindow.cpp \
    opendevicedialog.cpp \
    librarywindow.cpp

HEADERS  += devicewindow.h \
    opendevicedialog.h \
    librarywindow.h

FORMS    += devicewindow.ui \
    opendevicedialog.ui \
    librarywindow.ui
