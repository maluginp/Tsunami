#include "MosDevice.h"

namespace tsunami{
namespace spice{

MosDevice::MosDevice(const QString &name, DevicePolarity polarity)
    : Device(name), polarity_(polarity)
{
    setFlag( DEVICE_FLAG_NONLINEAR | DEVICE_FLAG_HAVE_MODEL );

    addTerminal("D");
    addTerminal("G");
    addTerminal("S");
    addTerminal("B");

    addParameter("M",QVariant::Double);
    addParameter("L",QVariant::Double);
    addParameter("W",QVariant::Double);
    addParameter("AD",QVariant::Double);
    addParameter("AS",QVariant::Double);
    addParameter("PD",QVariant::Double);
    addParameter("PS",QVariant::Double);
    addParameter("NRD",QVariant::Double);
    addParameter("NRS",QVariant::Double);
    addParameter("OFF",QVariant::Bool);
    addParameter("IC",QVariant::List);
    addParameter("TEMP",QVariant::Double);

}

DeviceType MosDevice::type() {
    if(polarity_ == POLARITY_P){
        return DEVICE_PMOS;
    }

    return DEVICE_NMOS;

}

QByteArray MosDevice::netlist() {

    QString nets;

    nets = QString("M%1 %2 %3 %4 %5").arg(id())
            .arg(terminal("D")->id())
            .arg(terminal("G")->id())
            .arg(terminal("S")->id())
            .arg(terminal("B")->id());


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
