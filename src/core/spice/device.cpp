#include "device.h"
#include "terminal.h"
#include "spicemodel.h"

namespace tsunami{
namespace core{

Device::Device(const QString &name, Device::TypeDevice device)
    : GraphNode(name)
    , device_(device)
    , model_(NULL)
    , flags_(0x0)
    , source_(SOURCE_UNKNOWN) {

}

void Device::setSource(Device::TypeSource source, const QVariantMap &options) {
    setFlag( DEVICE_FLAG_SOURCE  );
    sourceOptions_ = options;
}

bool Device::isFlagged(DeviceFlag flag) {
    return ((flags_ & flag ) == flag);
}

void Device::setFlag(DeviceFlag flag) {
    flags_ = flag;
}

void Device::addFlag(DeviceFlag flag) {
    flags_ |= flag;
}

void Device::connect(Terminal *terminal) {
    link(terminal);
    terminal->attachDevice( this );
}

void Device::disconnect(Terminal *terminal) {
    unlink(terminal);
    terminal->detachDevice(  this );
}

void Device::setModel(SpiceModel *model) {
    Q_ASSERT( model != NULL );
    model_ = model;
}

SpiceModel *Device::model() {
    Q_ASSERT( model_ != NULL );
    return model_;
}

QVector<Terminal *> Device::getTerminals() {
    QVector<GraphNode*> nodes = getNodes();
    QVector<Terminal*> terminals;
    int nNodes = nodes.size();
    for(int i=0; i < nNodes; ++i) {
        terminals.append( static_cast<Terminal*>(nodes[i]) );
    }

    return terminals;
}



}
}
