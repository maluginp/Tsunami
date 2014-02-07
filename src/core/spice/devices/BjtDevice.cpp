#include "BjtDevice.h"
namespace tsunami{
namespace spice{

BjtDevice::BjtDevice(const QString &name, DevicePolarity polarity)
    : Device(name), polarity_(polarity)
{
    setFlag( DEVICE_FLAG_NONLINEAR | DEVICE_FLAG_HAVE_MODEL );

    addTerminal( "C" );
    addTerminal( "B" );
    addTerminal( "E" );

    addParameter("AREA",QVariant::Double);
    addParameter("AREAC",QVariant::Double);
    addParameter("AREAB",QVariant::Double);
    addParameter("M",QVariant::Double);
    addParameter("OFF",QVariant::Bool);
    addParameter("IC",QVariant::List);
    addParameter("TEMP",QVariant::Double);
    addParameter("DTEMP",QVariant::Double);

}

DeviceType BjtDevice::type() {
    if(polarity_ == POLARITY_P){
        return DEVICE_PBJT;
    }

    return DEVICE_NBJT;
}

QByteArray BjtDevice::netlist() {

    QString nets;

    nets = QString("Q%1 %2 %3 %4").arg(id())
            .arg(terminal("C")->id())
            .arg(terminal("B")->id())
            .arg(terminal("E")->id());


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
