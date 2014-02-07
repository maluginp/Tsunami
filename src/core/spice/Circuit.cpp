#include "Circuit.h"

#include "Device.h"
#include "Terminal.h"
#include "SpiceModel.h"
#include <QMap>
#include <QDebug>
namespace tsunami{
namespace spice{



Circuit::Circuit(const QString &name)
    : name_(name)
{
     GraphNode::nextId_ = 1;
}


Circuit::~Circuit(){

}

bool Circuit::create(DeviceType type, SourceManager* sources) {
    clean();

    Device* device = addDeviceImpl( "device", type );
    // Ищем земли
    Device* gnd = addDeviceImpl( "gnd", DEVICE_GND );
    bool foundGround = false;
    foreach(Source* source, sources->inputs()){
        if(source->isGnd()){
            device->connect( source->node(), gnd->terminal(0) );
            foundGround = true;
        }else if(source->isCurrent()){
            Device* sourceDevice = addDeviceImpl("",DEVICE_ISOURCE);
            device->connect( source->node(), sourceDevice->terminal("P") );
            sourceDevice->connect( "M", gnd->terminal(0) );
        }else if(source->isVoltage()){
            Device* sourceDevice = addDeviceImpl("",DEVICE_VSOURCE);
            device->connect( source->node(), sourceDevice->terminal("P") );
            sourceDevice->connect( "M", gnd->terminal(0) );
        }
    }

    if(!foundGround){
        clean();
    }

    return foundGround;
}

Device* Circuit::addDeviceImpl(const QString &name, DeviceType type) {
    Device* device;
    int id;


    if(type == DEVICE_VSOURCE){

    }


    devices_.insert( id, device );
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
