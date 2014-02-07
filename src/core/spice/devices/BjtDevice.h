#ifndef BJTDEVICE_H
#define BJTDEVICE_H

#include "spice/Device.h"
namespace tsunami{
namespace spice{

class TSUNAMI_EXPORT BjtDevice : public Device {
public:
    BjtDevice(const QString& name, DevicePolarity polarity = POLARITY_N);
    DeviceType type();

    QByteArray netlist();

private:
    DevicePolarity polarity_;
};

}
}

#endif // BJTDEVICE_H
