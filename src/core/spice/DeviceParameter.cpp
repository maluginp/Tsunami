#include "DeviceParameter.h"

namespace tsunami{
namespace spice{

DeviceParameter::DeviceParameter(const QString &name, QVariant::Type type)
    : name_(name),type_(type),enable_(false)
{

}

bool DeviceParameter::enabled() {
    return enable_;
}

void DeviceParameter::enable(bool enabled) {
    enable_ = enabled;
}

void DeviceParameter::value(const QVariant &value) {
    value_ = value;
}

const QVariant &DeviceParameter::value() const {
    return value_;
}

const QString &DeviceParameter::name() const {
    return name_;
}


}
}
