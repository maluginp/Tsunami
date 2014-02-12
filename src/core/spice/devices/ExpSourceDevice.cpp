#include "ExpSourceDevice.h"

namespace tsunami{
namespace spice{

ExpSourceDevice::ExpSourceDevice(const QString &name, SourceMode type)
    : IndependentSourceDevice(name,type)
{
    if(type == SOURCE_MODE_CURRENT){
        addParameter("I1",QVariant::Double);
        addParameter("I2",QVariant::Double);
    }else{
        addParameter("V1",QVariant::Double);
        addParameter("V2",QVariant::Double);
    }
    addParameter("TD1",QVariant::Double);
    addParameter("TAU1",QVariant::Double);
    addParameter("TD2",QVariant::Double);
    addParameter("TAU2",QVariant::Double);

}

QByteArray ExpSourceDevice::netlist() {
    QString nets = IndependentSourceDevice::netlist();


    // параметры

    return nets.toAscii();
}

}
}
