#include "IndependentSourceDevice.h"
namespace tsunami{
namespace spice{

IndependentSourceDevice::IndependentSourceDevice(const QString &name, SourceMode mode)
    : Device(name),mode_(mode)
{
    setFlag( DEVICE_FLAG_SOURCE );

    addTerminal("P");
    addTerminal("M");

    addParameter("DC",      QVariant::Double);
    addParameter("ACMAG",   QVariant::Double);
    addParameter("ACPHASE", QVariant::Double);


}

DeviceType IndependentSourceDevice::type() {
    if(mode_ == SOURCE_MODE_CURRENT){
        return DEVICE_ISOURCE;
    }

    return DEVICE_VSOURCE;
}

SourceType IndependentSourceDevice::typeSource() {
    if(mode_ == SOURCE_MODE_GND) {
        return SOURCE_TYPE_GND;
    }
    return SOURCE_TYPE_CONST;
}

const SourceMode &IndependentSourceDevice::mode() const {
    return mode_;
}

void IndependentSourceDevice::mode(SourceMode mode) {
    mode_ = mode;
}

QByteArray IndependentSourceDevice::netlist() {

    QString type = (mode_ == SOURCE_MODE_CURRENT) ? "I" : "V";

    QString nets = QString("%1%2 %3 %4").arg(type).arg(id())
            .arg(terminal("P")->id()).arg(terminal("M")->id());

    // заполнить параметры
    return nets.toAscii();
}


}
}