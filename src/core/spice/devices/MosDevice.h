#ifndef MOSDEVICE_H
#define MOSDEVICE_H
#include "spice/Device.h"
namespace tsunami{
namespace spice{

class MosDevice : public Device {
public:
    MosDevice(const QString& name, DevicePolarity polarity = POLARITY_N);
    DeviceType type();

    QByteArray netlist();

private:
    DevicePolarity polarity_;
};

}
}
#endif // MOSDEVICE_H
