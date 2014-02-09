#include "PulseSourceDevice.h"
namespace tsunami{
namespace spice{

PulseSourceDevice::PulseSourceDevice(const QString& name, SourceMode type)
    : IndependentSourceDevice(name,type)
{
    addParameter("V1",QVariant::Double);
    addParameter("V2",QVariant::Double);
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
