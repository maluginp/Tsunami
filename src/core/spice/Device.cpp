#include "Device.h"
#include "Terminal.h"
#include "SpiceModel.h"

namespace tsunami{
namespace spice{

Device::Device(const QString &name)
    : GraphNode(name),
      flags_(0x0) {
}

void Device::addTerminal(const QString &name) {
    Terminal* terminal = new Terminal(name);
    if( name.compare("gnd") == 0 ){
        terminal->setRef(); // Делаем землёй
    }

    link(terminal);
    terminals_.append(name);
}

void Device::addParameter(const QString &name, QVariant::Type type){
    DeviceParameter parameter(name,type);
    parameters_.append( parameter );
}

bool Device::isFlagged(DeviceFlag flag) {
    return ((flags_ & flag ) == flag);
}

void Device::setFlag(DeviceFlag flag) {
    flags_ = flag;
}

void Device::connect(const QString &node, Terminal *terminal) {
    int index = terminals_.indexOf( node );
    Q_ASSERT(index >= 0);
    delete getNode(index);
    setNode(index,terminal);
}

void Device::setSpiceModel(SpiceModel *model) {
    model_ = model;
}

SpiceModel *Device::spiceModel() {
    return model_;
}

bool Device::hasSpiceModel() {
    if (isFlagged(DEVICE_FLAG_HAVE_MODEL)) {
        if (model_) {
            return true;
        }
    }
    return false;
}

Terminal *Device::terminal(const QString &node) {
    int index = terminals_.indexOf( node );

    Terminal* terminal = static_cast<Terminal*>( getNode(index) );

    return terminal;
}

const QList<DeviceParameter> &Device::parameters() const {
    return parameters_;
}

void Device::paramValue(const QString &name, const QVariant &value) {
    int numberParams = parameters_.count();
    for(int i=0; i < numberParams; ++i){
        if(parameters_[i].name().compare(name,Qt::CaseInsensitive)==0){
            parameters_[i].value(value);
        }
    }
}


//void Device::connect(Terminal *terminal) {
//    link(terminal);
//    terminal->attachDevice( this );
//}

//void Device::disconnect(Terminal *terminal) {
//    terminal->detachDevice(  this );
//    unlink(terminal);
//}



}
}
