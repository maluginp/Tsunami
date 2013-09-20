#include "devicemodel.h"

DeviceModel::DeviceModel()
    : deviceId_(-1), name_(QString()),
      nodes_(QStringList()), attributes_(QStringList()) {

}

DeviceModel::DeviceModel(const DeviceModel &other)
    : deviceId_(other.deviceId_), name_(other.name_),
      nodes_(other.nodes_), attributes_(other.attributes_) {

}

DeviceModel &DeviceModel::operator=(const DeviceModel &other) {
    deviceId_   = other.deviceId_;
    name_       = other.name_;
    nodes_      = other.nodes_;
    attributes_ = other.attributes_;

    return *this;
}

DeviceModel &DeviceModel::setId(const int &deviceId) {
    deviceId_ = deviceId;
    return *this;
}

DeviceModel &DeviceModel::setName(const QString &name) {
    name_ = name;
    return *this;
}

DeviceModel &DeviceModel::setNodes(const QStringList &nodes) {
    nodes_ = nodes;
    return *this;
}

DeviceModel &DeviceModel::addNode(const QString &node) {
    nodes_.append( node );
    return *this;
}

DeviceModel &DeviceModel::setAttributes(const QStringList &attributes) {
    attributes_ = attributes;
    return *this;
}

DeviceModel &DeviceModel::addAttribute(const QString &attribute) {
    attributes_.append( attribute );
    return *this;
}
