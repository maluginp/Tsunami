#include "circuit.h"

#include "device.h"
#include "terminal.h"
#include "spicemodel.h"
namespace tsunami{
namespace core{



Circuit::Circuit(const QString &name)
    : name_(name)
    , flag_(DEVICE_NO_FLAG)
{

}

int Circuit::addDevice(const QString &name, Device::TypeDevice type) {
    Device* device = new Device(name,type);

    devices_.insert( device->id(), device );
    return device->id();
}

void Circuit::removeDevice(int id) {
    DeviceMap::Iterator it = devices_.find( id );
    if( it != devices_.end() ){
        Device* device = static_cast<Device*>(it.value());
        devices_.erase( it );
        delete device;
    }
}

Device *Circuit::getDevice(const QString &name) {
    DeviceMap::Iterator it = devices_.begin();
    for(; it != devices_.end(); it++){
        Device* device = static_cast<Device*>(it.value());
        if(device->name().compare( name, Qt::CaseInsensitive ) == 0){
            return device;
        }
    }

    Q_ASSERT(false);

    return NULL;
}

Device *Circuit::getDevice(int deviceId) {
    DeviceMap::Iterator it = devices_.find( deviceId );
    if( it != devices_.end() ){
        Device* device = static_cast<Device*>(it.value());
        return device;
    }

    Q_ASSERT(false);
    return NULL;
}

Device *Circuit::nextDevice() {
    while( currentDevice_ != devices_.end() &&
           !(static_cast<Device*>(currentDevice_.value())->isFlagged(flag_))) {
        currentDevice_++;
    }
    if( currentDevice_ != devices_.end() ){
        Device* device = static_cast<Device*>(currentDevice_.value());
        currentDevice_++;
        return device;
    }

    Q_ASSERT(false);

    return NULL;
}

void Circuit::beginDevice(DeviceFlag flag) {
    flag_ = flag;
    currentDevice_ = devices_.begin();
}

int Circuit::addTerminal(const QString &name) {
    Terminal* terminal = new Terminal(name);
    terminals_.insert( terminal->id(), terminal );

    return terminal->id();
}

void Circuit::removeTerminal(int terminalId) {
    TerminalMap::Iterator it = terminals_.find( terminalId );

    if(it != terminals_.end()){
        Terminal* terminal = static_cast<Terminal*>(it.value());
        terminals_.erase( it );
        delete terminal;
    }
}

Terminal *Circuit::getTerminal(const QString &name) {
    TerminalMap::Iterator it = terminals_.begin();
    for(; it != terminals_.end(); it++){
        Terminal* terminal = static_cast<Terminal*>(it.value());
        if(terminal->name().compare( name, Qt::CaseInsensitive ) == 0){
            return terminal;
        }
    }

    Q_ASSERT(false);

    return NULL;

}

Terminal *Circuit::getTerminal(int terminalId) {
    TerminalMap::Iterator it = terminals_.find( terminalId_ );
    if(it != terminals_.end()){
        Terminal* terminal = static_cast<Terminal*>(it.value());
        return terminal;
    }

    Q_ASSERT(false);
    return NULL;
}

Terminal *Circuit::nextTerminal() {

    if( currentTerminal_ != terminals_.end() ){

        Terminal* terminal = static_cast<Terminal*>(currentTerminal_.value());
        currentTerminal_++;
        return termuinal;
    }

    Q_ASSERT(false);

    return NULL;
}

void Circuit::beginTerminal() {
    currentTerminal_ = terminals_.begin();
}

void Circuit::addSpiceModel(const QString &name, const QVariantMap &parameters) {

    if( isModelExist(name) ){
        return;
    }

    SpiceModel* model = new SpiceModel( name );
    if(parameters.size() > 0){
        model->addParameter( parameters );
    }

}

void Circuit::addSpiceModel(SpiceModel *model) {
    if( model == NULL || isModelExist(model->name()) ){
        return;
    }
    models_.append( model );
}

SpiceModel *Circuit::getModel(const QString &name) {
    ModelList::Iterator it = models_.begin();
    for(;it != models_.end(); it++){
        SpiceModel* model = static_cast<SpiceModel*>(it.reference);
        if(model->name().compare(name,Qt::CaseInsensitive) == 0){
            return model;
        }
    }

    Q_ASSERT(false);
    return NULL;
}

void Circuit::beginModel() {
    currentModel_ = models_.begin();
}

void Circuit::removeModel(const QString &name) {
    ModelList::Iterator it = models_.begin();
    for(;it != models_.end(); it++){
        SpiceModel* model = static_cast<SpiceModel*>(it.reference);
        if(model->name().compare(name,Qt::CaseInsensitive) == 0){
            models_.erase( it );
            break;
        }
    }

    return;
}

SpiceModel *Circuit::nextModel() {
    if(currentModel_ != models_.end()){
        SpiceModel* model = static_cast<SpiceModel*>(currentModel_.reference);
        currentModel_++;
        return model;
    }

    return NULL;
}

void Circuit::setRefTerminal(int terminalId) {
    Terminal* terminal = getTerminal(terminalId);
    if(terminal != NULL && !terminal->isRef()){
        terminal->setRef();
    }
}

void Circuit::connect(int deviceId, int terminalId) {
    Device* device = getDevice(deviceId);
    Terminal* terminal = getTerminal(terminalId);
    device->connect( terminal );
}

bool Circuit::isModelExist(const QString &name) {
    foreach(SpiceModel* model, models_){
        if(model->name().compare(name,Qt::CaseInsensitive) == 0){
            return true;
        }
    }
    return false;
}



}
}
