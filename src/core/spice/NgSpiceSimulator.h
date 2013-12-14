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
    QByteArray generateNetListModels( );
    QByteArray generateNetList();
    QByteArray generateNetPrints();

    QStringList columns_;

private:
    bool isCorrectForParse();

};

}
}

#endif // NGSPICESIMULATOR_H
