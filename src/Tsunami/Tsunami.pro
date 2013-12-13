#-------------------------------------------------
#
# Project created by QtCreator 2013-11-25T20:40:35
#
#-------------------------------------------------

TEMPLATE = app
include(../../Tsunami.pri)

CONFIG += gui

CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
}

TRANSLATIONS += i18n/tsunami_en.ts \
                i18n/tsunami_ru.ts

LIBS += -L$$DESTDIR -lcore -llogger


SOURCES += main.cpp\
    OpenMeasureDialog.cpp \
    ShowTextDialog.cpp \
    AnalysisWindow.cpp \
    SettingsDialog.cpp \
    AboutDialog.cpp \
    CreateDeviceDialog.cpp \
    CreateLibraryDialog.cpp \
    PlotExtractionDialog.cpp \
    DeviceWindow.cpp \
    ExtractorWindow.cpp \
    LibraryWindow.cpp \
    ChoiceAnalysisForm.cpp \
    AnalysisForm.cpp \
    AddMeasureForm.cpp \
    OpenDeviceDialog.cpp \
    OpenLibraryDialog.cpp \
    PlotDataWindow.cpp \
    PrepareExtractorDialog.cpp

HEADERS  += \
    OpenMeasureDialog.h \
    ShowTextDialog.h \
    AnalysisWindow.h \
    SettingsDialog.h \
    AboutDialog.h \
    CreateDeviceDialog.h \
    CreateLibraryDialog.h \
    PlotExtractionDialog.h \
    AddMeasureForm.h \
    AnalysisForm.h \
    ChoiceAnalysisForm.h \
    DeviceWindow.h \
    ExtractorWindow.h \
    LibraryWindow.h \
    OpenDeviceDialog.h \
    OpenLibraryDialog.h \
    PlotDataWindow.h \
    PrepareExtractorDialog.h

FORMS    += devicewindow.ui \
    opendevicedialog.ui \
    librarywindow.ui \
    openlibrarydialog.ui \
    prepareextractordialog.ui \
    extractorwindow.ui \
    plotdatawindow.ui \
    choiceanalysisform.ui \
    analysisform.ui \
    addmeasureform.ui \
    OpenMeasureDialog.ui \
    ShowTextDialog.ui \
    AnalysisWindow.ui \
    SettingsDialog.ui \
    AboutDialog.ui \
    CreateDeviceDialog.ui \
    CreateLibraryDialog.ui \
    PlotExtractionDialog.ui

RESOURCES += \
    resources.qrc
