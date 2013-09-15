include(../../Tsunami.pri)

CONFIG += gui qt sql

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
}


HEADERS += \
    ../../include/defines.h

SOURCES += src/main.cpp
