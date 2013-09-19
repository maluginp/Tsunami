#-------------------------------------------------
#
# Project created by QtCreator 2013-09-15T22:32:57
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET   =  core
CONFIG   += console sql
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    components/json.cpp \
    models/strategymodel.cpp \
    models/parametermodel.cpp \
    models/measuremodel.cpp \
    models/librarymodel.cpp \
    dbstorage/settingstorage.cpp \
    dbstorage/parameterstorage.cpp \
    dbstorage/measurestorage.cpp \
    dbstorage/dbstorage.cpp \
    views/measureitemview.cpp \
    views/parameteritemview.cpp \
    views/listitemview.cpp \
    models/measureplotmodel.cpp

HEADERS += \
    components/json.h \
    models/strategymodel.h \
    models/parametermodel.h \
    models/measuremodel.h \
    models/librarymodel.h \
    dbstorage/settingstorage.h \
    dbstorage/parameterstorage.h \
    dbstorage/measurestorage.h \
    dbstorage/dbstorage.h \
    views/measureitemview.h \
    views/parameteritemview.h \
    views/listitemview.h \
    models/measureplotmodel.h

include(../../Tsunami.pri)

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
}
