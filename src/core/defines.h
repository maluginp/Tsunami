#ifndef DEFINES_H
#define DEFINES_H

#include "math/matrix.h"
#include "math/vector.h"

namespace tsunami{
namespace core{

//template<class T> class Matrix;
//template<class T> class Vector;


enum{
    DEVICE_NO_FLAG      = 0,
    DEVICE_FLAG_SOURCE  = 1 << 0
};
typedef unsigned long DeviceFlag;

enum TypeDevice{
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
enum TypeAnalysis{
    ANALYSIS_UNKNOWN,
    ANALYSIS_AC,
    ANALYSIS_DC,
    ANALYSIS_TRAN
};

enum TypeSource{
    SOURCE_UNKNOWN,
    SOURCE_CONST,
    SOURCE_LINEAR
};

typedef Matrix<double> MatrixDouble;
typedef Matrix<int>    MatrixInt;
typedef Vector<double> VectorDouble;
typedef Vector<int>    VectorInt;


}
}

#endif // DEFINES_H
