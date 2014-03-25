#ifndef DEFINES_H
#define DEFINES_H

#include "export.h"

#include <QtCore>
#include <QtGui>

#include "math/Matrix.h"
#include "math/Vector.h"
#include "components/Source.h"
#include "utils.h"

namespace tsunami{

enum{
    DEVICE_NO_FLAG      = 0,
    DEVICE_FLAG_SOURCE  = 1 << 0,
    DEVICE_FLAG_NONLINEAR    = 1 << 1,
    DEVICE_FLAG_HAVE_MODEL = 1 << 2
};

typedef unsigned long DeviceFlag;


enum DevicePolarity{
    POLARITY_N,
    POLARITY_P
};


enum DeviceType{
    DEVICE_UNKNOWN=-1,
    DEVICE_NBJT,
    DEVICE_PBJT,
    DEVICE_NFET,
    DEVICE_PFET,
    DEVICE_NMOS,
    DEVICE_PMOS,
    DEVICE_DIODE,
    DEVICE_RESISTOR,
    DEVICE_ISOURCE,
    DEVICE_VSOURCE,
    DEVICE_CAPACITOR,
    DEVICE_GND
};
enum AnalysisType{
    ANALYSIS_UNKNOWN,
    ANALYSIS_AC,
    ANALYSIS_DC,
    ANALYSIS_TRAN
};



typedef core::Matrix<double> MatrixDouble;
typedef core::Matrix<int>    MatrixInt;
typedef core::Vector<double> VectorDouble;
typedef core::Vector<int>    VectorInt;



#include <limits>

#define TSUNAMI_DOUBLE_MAX std::numeric_limits<double>::max()
#define TSUNAMI_DOUBLE_MIN std::numeric_limits<double>::min()
#define TSUNAMI_DOUBLE_INF std::numeric_limits<double>::infinity()
#define TSUNAMI_DOUBLE_EPS std::numeric_limits<double>::epsilon()

}

#endif // DEFINES_H
