#include "DiodeDevice.h"

namespace tsunami{
namespace spice{

DiodeDevice::DiodeDevice(const QString &name)
    : Device(name)
{
    setFlag(DEVICE_FLAG_HAVE_MODEL | DEVICE_FLAG_NONLINEAR);
    addTerminal("P");
    addTerminal("M");

    addParameter("AREA",QVariant::Double);
    addParameter("M",QVariant::Double);
    addParameter("PJ",QVariant::Double);
    addParameter("OFF",QVariant::Bool);
    addParameter("IC",QVariant::Double);
    addParameter("TEMP",QVariant::Double);
    addParameter("DTEMP",QVariant::Double);

}

DeviceType DiodeDevice::type() {
    return DEVICE_DIODE;
}

QByteArray DiodeDevice::netlist() {
    QString nets;

    nets = QString("D%1 %2 %3").arg(id())
            .arg(terminal("P")->id())
            .arg(terminal("M")->id());


    if(hasSpiceModel()){
//        nets.append( spiceModel()-> )
    }

    foreach(DeviceParameter parameter,parameters()){
// Заполнение параметров
    }

    return nets.toAscii();
}

}
}
