#include "JfetDevice.h"

namespace tsunami{
namespace spice{

JfetDevice::JfetDevice(const QString &name, DevicePolarity polarity)
    : Device(name), polarity_(polarity)
{
    setFlag( DEVICE_FLAG_NONLINEAR | DEVICE_FLAG_HAVE_MODEL );

    addTerminal("D");
    addTerminal("G");
    addTerminal("S");

    addParameter("AREA",QVariant::Double);
    addParameter("OFF",QVariant::Bool);
    addParameter("IC",QVariant::List);
    addParameter("TEMP",QVariant::Double);
}

DeviceType JfetDevice::type() {
    if(polarity_ == POLARITY_P) {
        return DEVICE_PFET;
    }
    return DEVICE_NFET;
}

QByteArray JfetDevice::netlist() {
    QString nets;

    nets = QString("J%1 %2 %3 %4").arg(id())
            .arg(terminal("D")->id())
            .arg(terminal("G")->id())
            .arg(terminal("S")->id());


    if(hasSpiceModel()){
        nets.append(" ").append( spiceModel()->name() );
    }

    foreach(DeviceParameter parameter,parameters()){
// Заполнение параметров
    }

    return nets.toAscii();
}

}
}
