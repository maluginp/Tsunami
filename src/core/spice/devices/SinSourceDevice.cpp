#include "SinSourceDevice.h"
namespace tsunami{
namespace spice{

SinSourceDevice::SinSourceDevice(const QString &name, SourceMode type)
    : IndependentSourceDevice(name,type)
{
    if(type == SOURCE_MODE_CURRENT){
        addParameter("IO",QVariant::Double);
        addParameter("IA",QVariant::Double);
    }else{
        addParameter("VO",QVariant::Double);
        addParameter("VA",QVariant::Double);
    }
    addParameter("FREQ",QVariant::Double);
    addParameter("TD",QVariant::Double);
    addParameter("THETA",QVariant::Double);

}




QByteArray SinSourceDevice::netlist() {
    QString nets = IndependentSourceDevice::netlist();


    // параметры

    return nets.toAscii();
}

}
}
