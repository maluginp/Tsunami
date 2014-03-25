#-------------------------------------------------
#
# Project created by QtCreator 2014-03-25T22:23:44
#
#-------------------------------------------------
TEMPLATE = app
#QT       += core

include(../../Tsunami.pri)
#QT       -= gui

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
}

LIBS += -L$$DESTDIR -lcore -llogger

TARGET = experiments
CONFIG   += console
CONFIG   -= app_bundle




SOURCES += main.cpp \
    linear_search.cpp
