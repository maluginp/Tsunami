#include "PulseSourceDevice.h"
namespace tsunami{
namespace spice{

PulseSourceDevice::PulseSourceDevice(const QString& name, SourceMode type)
    : IndependentSourceDevice(name,type)
{
    if(type == SOURCE_MODE_CURRENT){
        addParameter("I1",QVariant::Double);
        addParameter("I2",QVariant::Double);
    }else{
        addParameter("V1",QVariant::Double);
        addParameter("V2",QVariant::Double);
    }
    addParameter("TD",QVariant::Double);
    addParameter("TR",QVariant::Double);
    addParameter("TF",QVariant::Double);
    addParameter("PW",QVariant::Double);
    addParameter("PER",QVariant::Double);


}

QByteArray PulseSourceDevice::netlist() {

    QString nets = IndependentSourceDevice::netlist();


    // параметры

    return nets.toAscii();
}

SourceType PulseSourceDevice::typeSource() {
    return SOURCE_TYPE_PULSE;
}

}
}
