#include "ngspicesimulator.h"
#include <QFile>

namespace tsunami{
namespace core{

NgSpiceSimulator::NgSpiceSimulator(const QString &path) :
    Simulator(path) {

}

bool NgSpiceSimulator::simulate() {

    // Generate netlist

    QString fileName = randomName(8);

    QFile file( QString("%1.net").arg(fileName) );

    if(!file.open(QIODevice::WriteOnly)){
        return false;
    }


//    generateDeviceNet()

    file.close();

    // Execute
    QStringList arguments;
    arguments << "-b" << QString("-o%1.out").arg(fileName)
                 << QString("%1.net").arg(fileName);

    if(!exec( arguments )){
        return false;
    }

    file.remove();

    if(!isCorrectForParse()){
        return false;
    }

    // Parse data from file
    QStringList columns;

    // Fill data table
    SimulatorResult result(columns);
    result.add(  );

    data_ = result;

    return true;
}

bool NgSpiceSimulator::isCorrectForParse() {
    return true;
}



}
}
