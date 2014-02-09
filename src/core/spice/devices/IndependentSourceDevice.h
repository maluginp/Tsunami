#ifndef INDEPENDENTSOURCEDEVICE_H
#define INDEPENDENTSOURCEDEVICE_H

#include "spice/Device.h"
namespace tsunami{
namespace spice{

class TSUNAMI_EXPORT IndependentSourceDevice : public Device {
public:
    IndependentSourceDevice(const QString& name, SourceMode mode);
    DeviceType type();

    virtual SourceType typeSource();

    const SourceMode& mode() const;
    void mode(SourceMode mode);

    virtual QByteArray netlist();
private:
    SourceMode mode_;
};

}
}
#endif // INDEPENDENTSOURCEDEVICE_H
