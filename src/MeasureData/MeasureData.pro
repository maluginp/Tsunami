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
    src/addmeasureform.h \
    src/analysisform.h \
    src/plotdatawindow.h \
    src/choiceanalysisform.h

SOURCES += src/main.cpp \
    src/addmeasureform.cpp \
    src/analysisform.cpp \
    src/plotdatawindow.cpp \
    src/choiceanalysisform.cpp

FORMS += \
    src/addmeasureform.ui \
    src/analysisform.ui \
    src/plotdatawindow.ui \
    src/choiceanalysisform.ui
