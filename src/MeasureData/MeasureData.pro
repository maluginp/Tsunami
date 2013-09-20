include(../../Tsunami.pri)

CONFIG += gui

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
}

LIBS += -L $$DESTDIR -lcore

HEADERS += \
    ../../include/defines.h \
    src/measuredatawindow.h \
    src/addmeasureform.h

SOURCES += src/main.cpp \
    src/measuredatawindow.cpp \
    src/addmeasureform.cpp

FORMS += \
    src/measuredatawindow.ui \
    src/addmeasureform.ui
