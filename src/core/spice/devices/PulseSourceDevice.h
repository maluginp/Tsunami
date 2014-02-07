#ifndef PULSESOURCEDEVICE_H
#define PULSESOURCEDEVICE_H

#include "IndependentSourceDevice.h"
namespace tsunami{
namespace spice{

class PulseSourceDevice : public IndependentSourceDevice {
public:
    PulseSourceDevice(const QString &name, SourceMode type);

    QByteArray netlist();
};

}
}

#endif // PULSESOURCEDEVICE_H
