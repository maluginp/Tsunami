#ifndef DEFINES_H
#define DEFINES_H

#include "math/matrix.h"
#include "math/vector.h"
#include <QVariantMap>
#include <QStringList>
#include "components/source.h"

namespace tsunami{

enum{
    DEVICE_NO_FLAG      = 0,
    DEVICE_FLAG_SOURCE  = 1 << 0
};
typedef unsigned long DeviceFlag;

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
    DEVICE_SOURCE,
    DEVICE_CAPACITOR
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





}

#endif // DEFINES_H
