#ifndef NGSPICESIMULATOR_H
#define NGSPICESIMULATOR_H

#include "simulator.h"

namespace tsunami{
namespace core{

class NgSpiceSimulator : public Simulator {
public:
    NgSpiceSimulator( const QString& path );

    bool simulate();
protected:
    QByteArray generateNetListModels( );
    QByteArray generateNetList();
private:
    bool isCorrectForParse();

};

}
}
#endif // NGSPICESIMULATOR_H
