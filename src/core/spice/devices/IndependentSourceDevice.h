#ifndef INDEPENDENTSOURCEDEVICE_H
#define INDEPENDENTSOURCEDEVICE_H

#include "spice/Device.h"
namespace tsunami{
namespace spice{

class IndependentSourceDevice : public Device {
public:
    IndependentSourceDevice(const QString& name, SourceMode type);
    DeviceType type();

    virtual QByteArray netlist();
private:
    SourceMode type_;
};

}
}
#endif // INDEPENDENTSOURCEDEVICE_H
