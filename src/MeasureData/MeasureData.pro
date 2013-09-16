include(../../Tsunami.pri)

CONFIG += gui qt sql

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
}


HEADERS += \
    ../../include/defines.h \
    src/measuredatawindow.h

SOURCES += src/main.cpp \
    src/measuredatawindow.cpp

FORMS += \
    src/measuredatawindow.ui
