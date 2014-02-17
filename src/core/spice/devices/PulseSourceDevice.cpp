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

    QString source = QString("PULSE(");

    if(mode() == SOURCE_MODE_CURRENT){
        source.append(QString("%1,%2")
                      .arg(paramValue("I1").toString())
                      .arg(paramValue("I2").toString()));
    }else{
        source.append(QString("%1,%2")
                      .arg(paramValue("V1").toString())
                      .arg(paramValue("V2").toString()));
    }
    source.append(QString(",%1,%2,%3,%4,%5)").arg(paramValue("TD").toString())
            .arg(paramValue("TR").toString())
            .arg(paramValue("TF").toString())
            .arg(paramValue("PW").toString())
            .arg(paramValue("PER").toString()));

    // параметры
    nets.append(" ").append(source);


    return nets.toAscii();
}

SourceType PulseSourceDevice::typeSource() {
    return SOURCE_TYPE_PULSE;
}

}
}
