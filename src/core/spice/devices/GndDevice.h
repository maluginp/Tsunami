#ifndef GNDDEVICE_H
#define GNDDEVICE_H

#include "spice/Device.h"
namespace tsunami{
namespace spice{

class TSUNAMI_EXPORT GndDevice : public Device {
public:
    GndDevice();
    DeviceType type();
    QByteArray netlist();
};

}
}
#endif // GNDDEVICE_H
