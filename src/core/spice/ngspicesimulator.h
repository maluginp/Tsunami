#ifndef NGSPICESIMULATOR_H
#define NGSPICESIMULATOR_H

#include "simulator.h"

namespace tsunami{
namespace core{

class NgSpiceSimulator : public Simulator {
public:
    NgSpiceSimulator( const QString& path );

    bool simulate();
private:
    bool isCorrectForParse();

};

}
}
#endif // NGSPICESIMULATOR_H
