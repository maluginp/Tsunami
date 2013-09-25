include(../../Tsunami.pri)

CONFIG += gui

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
}

LIBS += -L$$DESTDIR -lcore

HEADERS += \
    src/measuredatawindow.h \
    src/addmeasureform.h \
    src/analysisform.h \
    choiceanalysisdialog.h

SOURCES += src/main.cpp \
    src/measuredatawindow.cpp \
    src/addmeasureform.cpp \
    src/analysisform.cpp \
    choiceanalysisdialog.cpp

FORMS += \
    src/measuredatawindow.ui \
    src/addmeasureform.ui \
    src/analysisform.ui \
    choiceanalysisdialog.ui
