#-------------------------------------------------
#
# Project created by QtCreator 2013-12-21T13:27:33
#
#-------------------------------------------------
TEMPLATE = app
TARGET    = tests

include(../Tsunami.pri)
QT       += testlib
#QT       -= gui


CONFIG   += console
CONFIG   -= app_bundle

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}

LIBS += -L$$DESTDIR -lcore -llogger

SOURCES += main.cpp \  # tst_TestCore.cpp # \
    DeviceStorageTest.cpp

   # tst_TestDeviceStorage.cpp
#DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    DeviceStorageTest.h \
    AutoTest.h
