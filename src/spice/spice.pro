#-------------------------------------------------
#
# Project created by QtCreator 2013-11-03T20:48:53
#
#-------------------------------------------------

QT       -= gui

TARGET = spice
TEMPLATE = lib

DEFINES += SPICE_LIBRARY

SOURCES += spice.cpp \
    parameterset.cpp

HEADERS += spice.h\
        spice_global.h \
    parameterset.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
