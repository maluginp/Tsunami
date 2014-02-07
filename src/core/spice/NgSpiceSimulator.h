#ifndef NGSPICESIMULATOR_H
#define NGSPICESIMULATOR_H

#include "defines.h"
#include "Simulator.h"

namespace tsunami{
namespace spice{

class TSUNAMI_EXPORT NgSpiceSimulator : public Simulator {
public:
    NgSpiceSimulator( const QString& path );

    bool simulate();
protected:
    QStringList columns_;

private:
    bool isCorrectForParse();
    void parseSimulatedData(const QByteArray& outputData);
    bool isChangeSign( int numberColumn );

    QStringList changeSigns_;
};

}
}

#endif // NGSPICESIMULATOR_H
