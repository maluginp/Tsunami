#ifndef ANALYSISVOLTAGEPULSE_H
#define ANALYSISVOLTAGEPULSE_H

#include "IAnalysisSource.h"
//#include "spice/devices/VoltagePulseDevice.h"

namespace tsunami {

class AnalysisVoltagePulse : public IAnalysisSource {
//    Q_OBJECT
public:
    AnalysisVoltagePulse();

    gui::KeyValuePairs pairs() const;
private:
//    spice::VoltagePulseDevice source_;
};

}

#endif // ANALYSISVOLTAGEPULSE_H
