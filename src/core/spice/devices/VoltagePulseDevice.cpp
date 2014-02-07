#include "VoltagePulseDevice.h"

namespace tsunami{
namespace spice{

VoltagePulseDevice::VoltagePulseDevice()
    : Device(""),
      voltageInitial_(0.0),
      voltageOn_(0.0),
      timeDelay_(0.0),
      timeRise_(0.0),
      timeFall_(0.0),
      timeOn_(0.0),
      timePeriod_(0.0),
      numberCycles_(0)
{

}

}
}
