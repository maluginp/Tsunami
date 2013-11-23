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
    src/plotdatawindow.h \
    src/addplotitemdialog.h \
    src/choiceanalysisform.h

SOURCES += src/main.cpp \
    src/measuredatawindow.cpp \
    src/addmeasureform.cpp \
    src/analysisform.cpp \
    src/plotdatawindow.cpp \
    src/addplotitemdialog.cpp \
    src/choiceanalysisform.cpp

FORMS += \
    src/measuredatawindow.ui \
    src/addmeasureform.ui \
    src/analysisform.ui \
    src/plotdatawindow.ui \
    src/addplotitemdialog.ui \
    src/choiceanalysisform.ui
