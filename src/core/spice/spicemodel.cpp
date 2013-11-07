#include "spicemodel.h"

namespace tsunami{
namespace core{

SpiceModel::SpiceModel(const QString &name, TypeDevice type)
    : name_(name)
    , type_(type){

}

SpiceModel &SpiceModel::addParameter(const QString &name, const QVariant &value) {
    parameters_.insert( name, value );
    return *this;
}

SpiceModel &SpiceModel::addParameter(const QVariantMap &parameters) {
    foreach( QString key, parameters.keys()  ){
        parameters_.insert( key, parameters.value(key) );
    }
    return *this;
}

const QVariantMap &SpiceModel::parameters() const {
    return parameters_;
}

const QString &SpiceModel::name() const {
    return name_;
}

const TypeDevice &SpiceModel::typeDevice() const {
    return type_;
}

void SpiceModel::typeDevice(TypeDevice type) {
    type_ = type;
}

void SpiceModel::clear() {
    parameters_.clear();
}



}
}
