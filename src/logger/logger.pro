#-------------------------------------------------
#
# Project created by QtCreator 2013-12-01T17:23:39
#
#-------------------------------------------------

QT       -= gui

TARGET = logger
TEMPLATE = lib
#CONFIG += staticlib

include(../../Tsunami.pri)

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
CONFIG+= lib
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
CONFIG+= staticlib
}


SOURCES += logger.cpp

HEADERS += logger.h

