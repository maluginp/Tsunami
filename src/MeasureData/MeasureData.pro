######################################################################
# Automatically generated by qmake (2.01a) ?? ????. 24 15:36:10 2013
######################################################################

TEMPLATE = app
include(../../Tsunami.pri)

CONFIG += gui

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
}

LIBS += -L$$DESTDIR -lcore

# Input
HEADERS += ui_addplotitemdialog.h \
           ui_choiceanalysisdialog.h \
           ui_measuredatawindow.h \
           src/addmeasureform.h \
           src/analysisform.h \
           src/choiceanalysisform.h \
           src/plotdatawindow.h
FORMS += src/addmeasureform.ui \
         src/analysisform.ui \
         src/choiceanalysisform.ui \
         src/plotdatawindow.ui
SOURCES += src/addmeasureform.cpp \
           src/analysisform.cpp \
           src/choiceanalysisform.cpp \
           src/main.cpp \
           src/plotdatawindow.cpp
