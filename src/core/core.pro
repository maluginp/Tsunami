#-------------------------------------------------
#
# Project created by QtCreator 2013-09-15T22:32:57
#
#-------------------------------------------------

TARGET   =  core
TEMPLATE = lib


include(../../Tsunami.pri)

CONFIG(debug, debug|release) {
    CONFIG+= lib
    DESTDIR=$$SOLUTION_DIR/build/debug
    DEFINES  += TSUNAMI_MAKEDLL
    DEFINES  += QCUSTOMPLOT_COMPILE_LIBRARY
}
CONFIG(release, debug|release) {
    CONFIG+= staticlib
    DESTDIR=$$SOLUTION_DIR/build/release
}

LIBS += -L$$DESTDIR -llogger

SOURCES += main.cpp \
    components/Json.cpp \
    models/StrategyModel.cpp \
    models/ParameterModel.cpp \
    models/MeasureModel.cpp \
    models/LibraryModel.cpp \
    dbstorage/SettingStorage.cpp \
    dbstorage/ParameterStorage.cpp \
    dbstorage/MeasureStorage.cpp \
    dbstorage/DbStorage.cpp \
    views/MeasureItemView.cpp \
    views/ParameterItemView.cpp \
    views/ListItemView.cpp \
    views/KeyValueView.cpp \
    models/DeviceModel.cpp \
    models/AnalysisModel.cpp \
    dbstorage/AnalysisStorage.cpp \
    delegates/KeyValueDelegate.cpp \
    delegates/DelegateReadOnly.cpp \
    plots/TsunamiPlot.cpp \
    components/qcustomplot.cpp \
    plots/GraphicItem.cpp \
    views/ParameterPlotView.cpp \
    spice/Device.cpp \
    spice/Simulator.cpp \
    spice/NgSpiceSimulator.cpp \
    spice/GraphNode.cpp \
    spice/Terminal.cpp \
    spice/SpiceModel.cpp \
    spice/Circuit.cpp \
    extractor/Extractor.cpp \
    extractor/Dataset.cpp \
    dbstorage/DeviceStorage.cpp \
    components/Source.cpp \
    delegates/Delegatecheckbox.cpp \
    views/MeasureEnableView.cpp \
    views/ParametersExtractionView.cpp \
    extractor/ExtractorHookeJeeves.cpp \
    delegates/DelegateDoubleItem.cpp \
    webkit/WebKit.cpp \
    webkit/APIObject.cpp \
    views/ListTreeView.cpp \
    plots/PlotItem.cpp \
    models/LibraryTemplateModel.cpp \
    components/ParseMeasureModel.cpp \
    spice/devices/VoltagePulseDevice.cpp \
    spice/devices/BjtDevice.cpp \
    spice/DeviceParameter.cpp \
    spice/devices/JfetDevice.cpp \
    spice/devices/MosDevice.cpp \
    spice/devices/IndependentSourceDevice.cpp \
    spice/devices/PulseSourceDevice.cpp \
    spice/devices/SinSourceDevice.cpp \
    spice/devices/ExpSourceDevice.cpp \
    spice/devices/DiodeDevice.cpp

HEADERS += \
    components/Json.h \
    models/StrategyModel.h \
    models/ParameterModel.h \
    models/MeasureModel.h \
    models/LibraryModel.h \
    dbstorage/SettingStorage.h \
    dbstorage/ParameterStorage.h \
    dbstorage/MeasureStorage.h \
    dbstorage/DbStorage.h \
    views/MeasureItemView.h \
    views/ParameterItemView.h \
    views/ListItemView.h \
    views/KeyValueView.h \
    models/DeviceModel.h \
    models/AnalysisModel.h \
    dbstorage/AnalysisStorage.h \
    delegates/KeyValueDelegate.h \
    delegates/DelegateReadOnly.h \
    plots/TsunamiPlot.h \
    components/qcustomplot.h \
    plots/GraphicItem.h \
    views/ParameterPlotView.h \
    spice/Device.h \
    spice/Simulator.h \
    spice/NgSpiceSimulator.h \
    spice/GraphNode.h \
    spice/Terminal.h \
    spice/SpiceModel.h \
    spice/Circuit.h \
    math/Matrix.h \
    math/Vector.h \
    defines.h \
    extractor/Extractor.h \
    extractor/Dataset.h \
    dbstorage/DeviceStorage.h \
    components/Source.h \
    delegates/DelegateCheckBox.h \
    views/MeasureEnableView.h \
    views/ParametersExtractionView.h \
    extractor/ExtractorHookeJeeves.h \
    delegates/DelegateDoubleItem.h \
    webkit/WebKit.h \
    webkit/APIObject.h \
    dbstorage/DbStorages.h \
    views/Views.h \
    views/ListTreeView.h \
    plots/PlotItem.h \
    models/LibraryTemplateModel.h \
    components/ParseMeasureModel.h \
    spice/devices/VoltagePulseDevice.h \
    spice/devices/BjtDevice.h \
    spice/DeviceParameter.h \
    spice/devices/JfetDevice.h \
    spice/devices/MosDevice.h \
    spice/devices/IndependentSourceDevice.h \
    spice/devices/PulseSourceDevice.h \
    spice/devices/SinSourceDevice.h \
    spice/devices/ExpSourceDevice.h \
    spice/devices/DiodeDevice.h

