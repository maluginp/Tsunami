#include "IndependentSourceDevice.h"
namespace tsunami{
namespace spice{

IndependentSourceDevice::IndependentSourceDevice(const QString &name, SourceMode type)
    : Device(name),type_(type)
{
    setFlag( DEVICE_FLAG_SOURCE );

    addTerminal("P");
    addTerminal("M");

    addParameter("DC",QVariant::Double);
    addParameter("ACMAG",QVariant::Double);
    addParameter("ACPHASE",QVariant::Double);


}

DeviceType IndependentSourceDevice::type() {
    if(type_ == SOURCE_MODE_CURRENT){
        return DEVICE_ISOURCE;
    }

    return DEVICE_VSOURCE;
}

QByteArray IndependentSourceDevice::netlist() {

    QString type = (type_ == SOURCE_MODE_CURRENT) ? "I" : "V";

    QString nets = QString("%1%2 %3 %4").arg(type).arg(id())
            .arg(terminal("P")->id()).arg(terminal("M")->id());

    // заполнить параметры
    return nets.toAscii();
}


}
}
