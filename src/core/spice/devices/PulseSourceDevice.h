#ifndef PULSESOURCEDEVICE_H
#define PULSESOURCEDEVICE_H

#include "IndependentSourceDevice.h"
namespace tsunami{
namespace spice{

class TSUNAMI_EXPORT PulseSourceDevice : public IndependentSourceDevice {
public:
    PulseSourceDevice(const QString &name, SourceMode type);

    QByteArray netlist();
    SourceType typeSource();
};

}
}

#endif // PULSESOURCEDEVICE_H
