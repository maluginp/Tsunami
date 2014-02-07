#ifndef VOLTAGEPULSEDEVICE_H
#define VOLTAGEPULSEDEVICE_H

#include "spice/Device.h"

namespace tsunami{
namespace spice{

class TSUNAMI_EXPORT VoltagePulseDevice : public Device
{
public:
    VoltagePulseDevice();

    const double& voltageInitial() const{
        return voltageInitial_;
    }
    const double& voltageOn() const{
        return voltageOn_;
    }
    const double& timeDelay() const{
        return timeDelay_;
    }
    const double& timeRise() const{
        return timeRise_;
    }
    const double& timeFall() const{
        return timeFall_;
    }
    const double& timeOn() const{
        return timeOn_;
    }
    const double& timePeriod() const{
        return timePeriod_;
    }
    const int& numberCycles() const{
        return numberCycles_;
    }

private:
    double voltageInitial_;
    double voltageOn_;
    double timeDelay_;
    double timeRise_;
    double timeFall_;
    double timeOn_;
    double timePeriod_;
    int numberCycles_;
};

}
}

#endif // VOLTAGEPULSEDEVICE_H
