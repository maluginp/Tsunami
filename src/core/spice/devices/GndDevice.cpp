#include "GndDevice.h"
namespace tsunami{
namespace spice{

GndDevice::GndDevice()
    : Device("gnd")
{
    addTerminal("gnd");
    terminal("gnd")->setRef();
}

DeviceType GndDevice::type() {
    return DEVICE_GND;
}

QByteArray GndDevice::netlist() {
    return QByteArray();
}

}
}
