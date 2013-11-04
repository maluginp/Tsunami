#-------------------------------------------------
#
# Project created by QtCreator 2013-09-15T22:32:57
#
#-------------------------------------------------

include(../../Tsunami.pri)

TARGET   =  core
CONFIG(debug, debug|release) {
    CONFIG+= lib
}
CONFIG(release, debug|release) {
    CONFIG+= staticlib
}
TEMPLATE = lib

SOURCES += main.cpp \
    components/json.cpp \
    models/strategymodel.cpp \
    models/parametermodel.cpp \
    models/measuremodel.cpp \
    models/librarymodel.cpp \
    dbstorage/settingstorage.cpp \
    dbstorage/parameterstorage.cpp \
    dbstorage/measurestorage.cpp \
    dbstorage/dbstorage.cpp \
    views/measureitemview.cpp \
    views/parameteritemview.cpp \
    views/listitemview.cpp \
    models/measureplotmodel.cpp \
    views/keyvalueview.cpp \
    models/devicemodel.cpp \
    models/analysismodel.cpp \
    models/analysisitems.cpp \
    dbstorage/analysisstorage.cpp \
    delegates/keyvaluedelegate.cpp \
    delegates/delegatereadonly.cpp \
    plots/tsunamiplot.cpp \
    components/qcustomplot.cpp \
    plots/graphicitem.cpp \
    views/parameterplotview.cpp \
    spice/parameterset.cpp \
    spice/source.cpp \
    spice/device.cpp \
    spice/simulator.cpp

HEADERS += \
    components/json.h \
    models/strategymodel.h \
    models/parametermodel.h \
    models/measuremodel.h \
    models/librarymodel.h \
    dbstorage/settingstorage.h \
    dbstorage/parameterstorage.h \
    dbstorage/measurestorage.h \
    dbstorage/dbstorage.h \
    views/measureitemview.h \
    views/parameteritemview.h \
    views/listitemview.h \
    models/measureplotmodel.h \
    views/keyvalueview.h \
    models/devicemodel.h \
    models/analysismodel.h \
    models/analysisitems.h \
    dbstorage/analysisstorage.h \
    delegates/keyvaluedelegate.h \
    delegates/delegatereadonly.h \
    plots/tsunamiplot.h \
    components/qcustomplot.h \
    plots/graphicitem.h \
    views/parameterplotview.h \
    spice/parameterset.h \
    spice/source.h \
    spice/device.h \
    spice/simulator.h


CONFIG(debug, debug|release) {
    DESTDIR=$$SOLUTION_DIR/build/debug
}
CONFIG(release,debug|release){
    DESTDIR=$$SOLUTION_DIR/build/release
}
