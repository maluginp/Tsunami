#ifndef DIODEDEVICE_H
#define DIODEDEVICE_H

#include "spice/Device.h"
namespace tsunami{
namespace spice{

class DiodeDevice : public Device {
public:
    DiodeDevice(const QString& name);
    DeviceType type();

    QByteArray netlist();
};

} // spice
} // tsunami
#endif // DIODEDEVICE_H
