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
    QString devName = name();

    if(devName.isEmpty()){
        QString type = (mode_ == SOURCE_MODE_CURRENT) ? "I" : "V";
        devName = QString("%1%2").arg(type).arg(id());
    }

    QString nets = QString("%1 %2 %3").arg(devName)
            .arg(terminal("P")->id()).arg(terminal("M")->id());

    if(hasParameter("dc")){
        nets.append( QString(" DC %1").arg(paramValue("dc").toString()) );
    }else{
        nets.append(QString(" AC %1 %2")
                    .arg(paramValue("acmag").toString())
                    .arg(paramValue("acphase").toString()));
    }
    // заполнить параметры
    return nets.toAscii();
}


}
}
