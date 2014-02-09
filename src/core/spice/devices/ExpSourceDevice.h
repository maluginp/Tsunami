#ifndef EXPSOURCEDEVICE_H
#define EXPSOURCEDEVICE_H

#include "IndependentSourceDevice.h"
namespace tsunami{
namespace spice{

class TSUNAMI_EXPORT ExpSourceDevice : public IndependentSourceDevice {
public:
    ExpSourceDevice(const QString &name, SourceMode type);

    QByteArray netlist();
};

}
}
#endif // EXPSOURCEDEVICE_H
