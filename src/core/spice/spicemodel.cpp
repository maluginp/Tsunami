#include "spicemodel.h"

namespace tsunami{
namespace spice{

SpiceModel::SpiceModel(const QString &name, DeviceType type)
    : name_(name)
    , type_(type){

}

void SpiceModel::add(const QString &name, const QVariant &value) {
    parameters_.insert( name, value );
    return;
}

void SpiceModel::add(const QVariantMap &parameters) {
    foreach( QString key, parameters.keys()  ){
        parameters_.insert( key, parameters.value(key) );
    }
    return;
}

void SpiceModel::set(const QString &name, const QVariant &value) {
    if(parameters_.contains( name )){
        parameters_[name] = value;
    }
}

QVariant SpiceModel::get(const QString &name) {
    return parameters_.value( name, QVariant() );
}

const QVariantMap &SpiceModel::parameters() const {
    return parameters_;
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

void SpiceModel::clear() {
    parameters_.clear();
}

bool SpiceModel::isFixed(const QString& parameter) {
    if(constraints_.contains( parameter )){
        ConstraintSpiceParameter constraint = constraints_.value(parameter);

        return constraint.fixed;
    }
    return true;
}

ConstraintSpiceParameter SpiceModel::constraint(const QString &parameter) {
    return constraints_.value(parameter, ConstraintSpiceParameter());
}



}
}
