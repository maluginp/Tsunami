#include "devicemodel.h"


namespace tsunami{
namespace db{

void DeviceModel::type(const QString &type){
    if(type.compare("nbjt")==0){
        type_ = DEVICE_NBJT;
    }else if(type.compare("pbjt") == 0){
        type_ = DEVICE_PBJT;
    }else if(type.compare("nfet") == 0){
        type_ = DEVICE_NFET;
    }else if(type.compare("pfet") == 0){
        type_ = DEVICE_PFET;
    }else if(type.compare("nmos") == 0){
        type_ = DEVICE_NMOS;
    }else if(type.compare("pmos") == 0){
        type_ = DEVICE_PMOS;
    }else if(type.compare("diode") == 0){
        type_ = DEVICE_DIODE;
    }else if(type.compare("res") == 0){
        type_ = DEVICE_RESISTOR;
    }else if(type.compare("source") == 0){
        type_ = DEVICE_SOURCE;
    }else if(type.compare("cap") == 0){
        type_ = DEVICE_CAPACITOR;
    }else{
        Q_ASSERT(false);
    }
}

QString DeviceModel::typeJson() const {

    switch(type_){
    case DEVICE_NBJT : return QString("nbjt");
    case DEVICE_PBJT : return QString("pbjt");
    case DEVICE_NFET : return QString("nfet");
    case DEVICE_PFET : return QString("pfet");
    case DEVICE_NMOS : return QString("nmos");
    case DEVICE_PMOS : return QString("pmos");
    case DEVICE_DIODE : return QString("diode");
    case DEVICE_RESISTOR : return QString("res");
    case DEVICE_SOURCE : return QString("source");
    case DEVICE_CAPACITOR : return QString("cap");
    }

    Q_ASSERT(false);
    return QString();
}

QStringList DeviceModel::nodes() {
    switch(type_){
    case DEVICE_NBJT :
    case DEVICE_PBJT :
        return QStringList() << "E" << "B" << "C";
    case DEVICE_NFET:
    case DEVICE_PFET:
    case DEVICE_NMOS:
    case DEVICE_PMOS:
        return QStringList() << "S" << "G" << "D" << "B";
    case DEVICE_DIODE:
    case DEVICE_RESISTOR:
    case DEVICE_CAPACITOR:
    case DEVICE_SOURCE:
        return QStringList() << "M" << "P";

    }

    return QStringList();
}

}
}
