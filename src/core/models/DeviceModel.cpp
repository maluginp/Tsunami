#include "DeviceModel.h"

namespace tsunami{
namespace db{

DeviceModel::DeviceModel() :
     deviceId_(-1),
     type_(DEVICE_UNKNOWN),
     createdAt_(QDateTime::currentDateTime()),
     changedAt_(QDateTime::currentDateTime()),
     enable_(true) { }

DeviceModel::DeviceModel(const DeviceModel &other) :
    deviceId_(other.deviceId_),
    name_(other.name_),
    type_(other.type_),
    model_(other.model_),
    createdAt_(other.createdAt_),
    changedAt_(other.changedAt_),
    enable_(other.enable_) { }

DeviceModel &DeviceModel::operator=(const DeviceModel &other) {
    deviceId_   = other.deviceId_;
    name_       = other.name_;
    type_       = other.type_;
    model_      = other.model_;
    createdAt_  = other.createdAt_;
    changedAt_  = other.changedAt_;
    enable_     = other.enable_;

    return *this;
}

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
    }else if(type.compare("isource") == 0){
        type_ = DEVICE_ISOURCE;
    }else if(type.compare("vsource") == 0){
        type_ = DEVICE_VSOURCE;
    }else if(type.compare("cap") == 0){
        type_ = DEVICE_CAPACITOR;
    }else{
        Q_ASSERT(false);
    }
}

QString DeviceModel::typeJson() const {

    switch(type_){
    case DEVICE_NBJT      : return QString("nbjt");
    case DEVICE_PBJT      : return QString("pbjt");
    case DEVICE_NFET      : return QString("nfet");
    case DEVICE_PFET      : return QString("pfet");
    case DEVICE_NMOS      : return QString("nmos");
    case DEVICE_PMOS      : return QString("pmos");
    case DEVICE_DIODE     : return QString("diode");
    case DEVICE_RESISTOR  : return QString("res");
    case DEVICE_ISOURCE   : return QString("isource");
    case DEVICE_VSOURCE   : return QString("vsource");
    case DEVICE_CAPACITOR : return QString("cap");
    case DEVICE_UNKNOWN:
    default:
        break;
    }

    Q_ASSERT(false);
    return QString();
}

QStringList DeviceModel::nodes() {
    switch(type_){
    case DEVICE_NBJT :
    case DEVICE_PBJT :
        return QStringList() << "C" << "B" << "E";
    case DEVICE_NFET:
    case DEVICE_PFET:
        return QStringList() << "D" << "G" << "S";
    case DEVICE_NMOS:
    case DEVICE_PMOS:
        return QStringList() << "D" << "G" << "S" << "B";
    case DEVICE_DIODE:
    case DEVICE_RESISTOR:
    case DEVICE_CAPACITOR:
    case DEVICE_ISOURCE:
    case DEVICE_VSOURCE:
        return QStringList() << "P" << "M";
    case DEVICE_UNKNOWN:
    default:
        break;
    }
    Q_ASSERT(false);
    return QStringList();
}

QString DeviceModel::debug() const {
    QString debug = QString("Device Model\n"
                            "ID: %1, NAME: %2, TYPE: %3, MODEL: %4, "
                            "ENABLE: %5, CREATED: %6, CHANGED: %7")
            .arg(deviceId_).arg(name_).arg(typeJson()).arg(model_).arg(enable_)
            .arg(createdAt_.toString())
            .arg(changedAt_.toString());

    return debug;
}

// FIXME: Add translations
QString DeviceModel::modelNameToTitle(const QString &modelName) {
    if(modelName == "gummelpoon") {
        return QString("Gummel-Poon");
    }
    QString name = modelName;
    return name.toUpper();
}

DeviceType DeviceModel::nameToType(const QString &name) {
    DeviceType type;
    if(name.compare("nbjt")==0){
        type = DEVICE_NBJT;
    }else if(name.compare("pbjt") == 0){
        type = DEVICE_PBJT;
    }else if(name.compare("nfet") == 0){
        type = DEVICE_NFET;
    }else if(name.compare("pfet") == 0){
        type = DEVICE_PFET;
    }else if(name.compare("nmos") == 0){
        type = DEVICE_NMOS;
    }else if(name.compare("pmos") == 0){
        type = DEVICE_PMOS;
    }else if(name.compare("diode") == 0){
        type = DEVICE_DIODE;
    }else if(name.compare("res") == 0){
        type = DEVICE_RESISTOR;
    }else if(name.compare("isource") == 0){
        type = DEVICE_ISOURCE;
    }else if(name.compare("vsource") == 0){
        type = DEVICE_VSOURCE;
    }else if(name.compare("cap") == 0){
        type = DEVICE_CAPACITOR;
    }else{
        type = DEVICE_UNKNOWN;
    }

    return type;
}

QString DeviceModel::typeToName(DeviceType type) {
    switch(type){
    case DEVICE_NBJT      : return QString("nbjt");
    case DEVICE_PBJT      : return QString("pbjt");
    case DEVICE_NFET      : return QString("nfet");
    case DEVICE_PFET      : return QString("pfet");
    case DEVICE_NMOS      : return QString("nmos");
    case DEVICE_PMOS      : return QString("pmos");
    case DEVICE_DIODE     : return QString("diode");
    case DEVICE_RESISTOR  : return QString("res");
    case DEVICE_ISOURCE   : return QString("isource");
    case DEVICE_VSOURCE   : return QString("vsource");
    case DEVICE_CAPACITOR : return QString("cap");
    case DEVICE_UNKNOWN:
    default:
        break;
    }

    return QString();
}

} // db
} // tsunami
