#include "Circuit.h"

#include "Device.h"
#include "Terminal.h"
#include "SpiceModel.h"
#include <QMap>
#include <QDebug>

#include "spice/Devices.h"

namespace tsunami{
namespace spice{



Circuit::Circuit(const QString &name)
    : name_(name)
{
     GraphNode::nextId_ = 1;
}


Circuit::~Circuit(){

}

const QString &Circuit::name() const {
    return name_;
}

bool Circuit::create(DeviceType type, const QList<Source*>& sources, SpiceModel* model) {
    clean();

    SourceManager *sourceManager = new SourceManager(sources);

    Device* device = addDeviceImpl( "device", type, model );
    // Ищем земли
    Device* gnd = addDeviceImpl( "gnd", DEVICE_GND );
    bool foundGround = false;
    foreach(Source* source, sourceManager->inputs()){
        if(source->isGnd()){
            device->connect( source->node(), gnd->terminal("gnd") );
            foundGround = true;
        }else if(source->isCurrent()){
            Device* sourceDevice = addSourceImpl(source->name(),DEVICE_ISOURCE,source);
            device->connect( source->node(), sourceDevice->terminal("P") );
            sourceDevice->connect( "M", gnd->terminal("gnd") );
        }else if(source->isVoltage()){
            Device* sourceDevice = addSourceImpl(source->name(),DEVICE_VSOURCE,source);
            device->connect( source->node(), sourceDevice->terminal("P") );
            sourceDevice->connect( "M", gnd->terminal("gnd") );
        }
    }
    delete sourceManager;

    if(!foundGround){
        clean();
    }

    return foundGround;
}

Device* Circuit::addDeviceImpl(const QString &name, DeviceType type, SpiceModel* model) {
    Device* device;
    int id;


    if(type == DEVICE_GND){
        device = new GndDevice();
    }else if(type ==  DEVICE_PFET){
        device = new JfetDevice(name,POLARITY_P);
    }else if(type == DEVICE_NFET){
        device = new JfetDevice(name,POLARITY_N);
    }else if(type == DEVICE_NBJT){
        device = new BjtDevice(name,POLARITY_N);
    }else if(type == DEVICE_PBJT){
        device = new BjtDevice(name,POLARITY_P);
    }else if(type == DEVICE_NMOS){
        device = new MosDevice(name,POLARITY_N);
    }else if(type == DEVICE_PMOS){
        device = new MosDevice(name,POLARITY_P);
    }

    if(model){
        device->setSpiceModel(model);
    }

    devices_.insert( device->id(), device );
    return device;
}

Device *Circuit::addSourceImpl(const QString &name, DeviceType type, Source *source) {
    Device* device = NULL;
    SourceMode mode;
    if(type == DEVICE_VSOURCE) {
        mode = SOURCE_MODE_VOLTAGE;
    }else{
        mode = SOURCE_MODE_CURRENT;
    }

    if(source->type() == SOURCE_TYPE_CONST){
        device = new IndependentSourceDevice(name,mode);
    }else if(source->type() == SOURCE_TYPE_PULSE){
        device = new PulseSourceDevice(name,mode);

        if(mode == SOURCE_MODE_CURRENT){
            device->paramValue("I1",source->configuration("I1",.0));
            device->paramValue("I2",source->configuration("I2",.0));
        }else{
            device->paramValue("V1",source->configuration("V1",.0));
            device->paramValue("V2",source->configuration("V2",.0));
        }
        device->paramValue("TD",source->configuration("TD",.0));
        device->paramValue("TR",source->configuration("TR",.0));
        device->paramValue("TF",source->configuration("TF",.0));
        device->paramValue("PW",source->configuration("PW",.0));
        device->paramValue("PER",source->configuration("PER",.0));
    }else if(source->type() == SOURCE_TYPE_SIN){
        device = new SinSourceDevice(name,mode);
        if(mode == SOURCE_MODE_CURRENT){
            device->paramValue("IO",source->configuration("IO",.0));
            device->paramValue("IA",source->configuration("IA",.0));
        }else{
            device->paramValue("VO",source->configuration("VO",.0));
            device->paramValue("VA",source->configuration("VA",.0));
        }
        device->paramValue("FREQ",source->configuration("FREQ",.0));
        device->paramValue("TD",source->configuration("TD",.0));
        device->paramValue("THETA",source->configuration("THETA",.0));
    }else if(source->type() == SOURCE_TYPE_EXP){
        device = new ExpSourceDevice(name,mode);
        if(mode == SOURCE_MODE_CURRENT){
            device->paramValue("I1",source->configuration("I1",.0));
            device->paramValue("I2",source->configuration("I2",.0));
        }else{
            device->paramValue("V1",source->configuration("V1",.0));
            device->paramValue("V2",source->configuration("V2",.0));
        }

        device->paramValue("TD1",source->configuration("TD1",.0));
        device->paramValue("TAU1",source->configuration("TAU1",.0));
        device->paramValue("TD2",source->configuration("TD2",.0));
        device->paramValue("TAU2",source->configuration("TAU2",.0));
    }else{
        Q_ASSERT(false);
    }

    device->paramValue( "dc", source->configuration("dc",.0) );
    device->paramValue( "acmag", source->configuration("acmag",.0) );
    device->paramValue( "acphase", source->configuration("acphase",.0) );

    devices_.insert(device->id(),device);
    return device;
}

void Circuit::clean() {
    qDeleteAll( devices_.values() );
    devices_.clear();
    GraphNode::nextId_ = 1;
}

QList<Device *> Circuit::getDevices(DeviceFlag flag) {
    QList<Device*> devices;
    foreach(Device* device, devices_.values()){
        if(device->isFlagged(flag)){
            devices.append( device );
        }
    }
    return devices;
}


//Device *Circuit::getDevice(const QString &name) {
//    DeviceMap::iterator it = devices_.begin();
//    for(; it != devices_.end(); it++){
//        Device* device = (*it).second;
//        if(device->name().compare( name, Qt::CaseInsensitive ) == 0){
//            return device;
//        }
//    }

//    Q_ASSERT(false);

//    return NULL;
//}




} // spice
} // tsunami
