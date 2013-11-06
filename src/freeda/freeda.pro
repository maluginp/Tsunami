#-------------------------------------------------
#
# Project created by QtCreator 2013-09-10T12:00:36
#
#-------------------------------------------------

include(../../Tsunami.pri)

TARGET   =  freeda
CONFIG(debug, debug|release) {
    CONFIG+= lib
}
CONFIG(release, debug|release) {
    CONFIG+= staticlib
}
TEMPLATE = lib


SOURCES += main.cpp \
    network/Terminal.cc \
    network/templates.cc \
    network/Polynomial.cc \
    network/NetListItem.cc \
    network/GraphNode.cc \
    network/ElementManager.cc \
    network/ElementData.cc \
    network/Element.cc \
    network/constants.cc \
    network/CircuitManager.cc \
    network/Circuit.cc \
    network/ADInterface.cc \
    analysis/Trapezoidal.cc \
    analysis/Tincidence.cc \
    analysis/TimeMNAM.cc \
    analysis/TimeDomainSV.cc \
    analysis/SVTran2.cc \
    analysis/SVTran.cc \
    analysis/SVTr.cc \
    analysis/slist.cc \
    analysis/Nox_Interface.cc \
    analysis/iohb.c \
    analysis/FreqMNAM.cc \
    analysis/FreqDomainSV.cc \
    analysis/Euler.cc \
    analysis/DC.cc \
    analysis/CircVector.cc \
    analysis/Analysis.cc \
    analysis/AC.cc

HEADERS += \
    network/TerminalData.h \
    network/Terminal.h \
    network/Polynomial.h \
    network/NetListItem.h \
    network/Instanciable.h \
    network/GraphNode.h \
    network/ElementManager.h \
    network/ElementData.h \
    network/Element.h \
    network/CircuitManager.h \
    network/Circuit.h \
    network/ADInterface.h \
    analysis/Trapezoidal.h \
    analysis/TimeMNAM.h \
    analysis/TimeDomainSV.h \
    analysis/SVTran2.h \
    analysis/SVTran.h \
    analysis/SVTr.h \
    analysis/slist.h \
    analysis/OFunction_Nox.h \
    analysis/Nox_Interface.h \
    analysis/iohb.h \
    analysis/integMethod.h \
    analysis/FreqMNAM.h \
    analysis/FreqDomainSV.h \
    analysis/Euler.h \
    analysis/DC.h \
    analysis/CircVector.h \
    analysis/Analysis.h \
    analysis/AC.h \
    containers.h

INCLUDEPATH += "C:/Develop/fftw-3.3.3/x86" \
               "C:/Develop/trilinos-10.2.2/build_2008/include"

DEFINES += ICL

LIBS += -L"$$PWD/libs/CLAPACK" \
        -l"BLAS_nowrap" -l"BLAS" -l"libf2c" -l"clapack" \
        -L"$$PWD/libs/trilinos" \
        -l"teuchos" -l"epetra" -l"epetraext" -l"nox" -l"noxepetra" -l"ifpack" -l"ml" \
        -l"triutils" -l"amesos" -l"aztecoo" \
        -L"$$PWD/libs/fftw3" \
        -l"libfftw3-3" -l"libfftw3f-3" \
        "/NODEFAULTLIB:libcmt" \
        "/MTd"

#QMAKE_CFLAGS_RELEASE = /MD
#QMAKE_CXXFLAGS_RELEASE = /MD
#QMAKE_CFLAGS_DEBUG = /MTd
#QMAKE_CXXFLAGS_DEBUG = /MTd
