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
    MeasureWindow.cpp \
    OpenDeviceDialog.cpp \
    OpenLibraryDialog.cpp \
    PlotDataWindow.cpp \
    PrepareExtractorDialog.cpp \
    sources/IAnalysisSource.cpp \
    sources/AnalysisVoltagePulse.cpp \
    analysis/IndepedentSourceDialog.cpp

HEADERS  += \
    OpenMeasureDialog.h \
    ShowTextDialog.h \
    AnalysisWindow.h \
    SettingsDialog.h \
    AboutDialog.h \
    CreateDeviceDialog.h \
    CreateLibraryDialog.h \
    PlotExtractionDialog.h \
    MeasureWindow.h \
    ChoiceAnalysisForm.h \
    DeviceWindow.h \
    ExtractorWindow.h \
    LibraryWindow.h \
    OpenDeviceDialog.h \
    OpenLibraryDialog.h \
    PlotDataWindow.h \
    PrepareExtractorDialog.h \
    sources/IAnalysisSource.h \
    sources/AnalysisVoltagePulse.h \
    analysis/IndepedentSourceDialog.h

FORMS    += DeviceWindow.ui \
    OpenDeviceDialog.ui \
    LibraryWindow.ui \
    OpenLibraryDialog.ui \
    PrepareExtractorDialog.ui \
    ExtractorWindow.ui \
    PlotDataWindow.ui \
    ChoiceAnalysisForm.ui \
    AnalysisForm.ui \
    MeasureWindow.ui \
    OpenMeasureDialog.ui \
    ShowTextDialog.ui \
    AnalysisWindow.ui \
    SettingsDialog.ui \
    AboutDialog.ui \
    CreateDeviceDialog.ui \
    CreateLibraryDialog.ui \
    PlotExtractionDialog.ui \
    analysis/IndepedentSourceDialog.ui

RESOURCES += \
    resources.qrc
