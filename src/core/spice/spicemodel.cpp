#include "spicemodel.h"
#include "models/librarymodel.h"
#include "models/parametermodel.h"

namespace tsunami{
namespace spice{

SpiceModel::SpiceModel(const QString &name, DeviceType type)
    : name_(name)
    , type_(type), library_(NULL){

}

void SpiceModel::setLibrary(db::LibraryModel *library) {
    library_ = library;
}

const QString &SpiceModel::name() const {
    return name_;
}

const DeviceType &SpiceModel::typeDevice() const {
    return type_;
}

void SpiceModel::typeDevice(DeviceType type) {
    type_ = type;
}

QByteArray SpiceModel::generateNetList() {
    QByteArray netlist;

    Q_ASSERT( library_ != NULL );

    netlist.append( QString(".model %1").arg(name_) );
    switch(type_){
    case DEVICE_CAPACITOR:
    case DEVICE_RESISTOR:
    case DEVICE_DIODE:
        netlist.append( "" );
        break;
    case DEVICE_NBJT:
        netlist.append(" NPN");
        break;
    case DEVICE_PBJT:
        netlist.append(" PNP");
        break;
    case DEVICE_NMOS:
        netlist.append(" NMOS");
        break;
    case DEVICE_PMOS:
        netlist.append(" PMOS");
        break;
    case DEVICE_UNKNOWN:
    default:
        Q_ASSERT(false);
//            break;
    }

    netlist.append("(");
    foreach(db::ParameterModel param, library_->parameters()){
        if(param.enable()){
            netlist.append( QString("%1=%2 ").arg(param.name()).arg(param.fitted()) );
        }
    }
    netlist.append(")\n");

    return netlist;

}


}
}
