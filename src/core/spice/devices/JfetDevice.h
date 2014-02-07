#ifndef JFETDEVICE_H
#define JFETDEVICE_H

#include "spice/Device.h"

namespace tsunami{
namespace spice{

class JfetDevice : public Device
{
public:
    JfetDevice(const QString& name, DevicePolarity polarity = POLARITY_N);
    DeviceType type();

    QByteArray netlist();

private:
    DevicePolarity polarity_;
};

} // spice
} // tsunami


#endif // JFETDEVICE_H
