#ifndef SINSOURCEDEVICE_H
#define SINSOURCEDEVICE_H

#include "IndependentSourceDevice.h"
namespace tsunami{
namespace spice{

class TSUNAMI_EXPORT SinSourceDevice : public IndependentSourceDevice {
public:
    SinSourceDevice(const QString &name, SourceMode type);

    QByteArray netlist();
};

}
}
#endif // SINSOURCEDEVICE_H
