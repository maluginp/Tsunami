#include "ngspicesimulator.h"
#include <QFile>
#include "circuit.h"
#include "spicemodel.h"
#include "device.h"

namespace tsunami{
namespace spice{

NgSpiceSimulator::NgSpiceSimulator(const QString &path) :
    Simulator(path) {

}

bool NgSpiceSimulator::simulate() {

    // Append models;
    QString fileName = randomName(8);

    QFile file( QString("%1.net").arg(fileName) );

    if(!file.open(QIODevice::WriteOnly)){
        return false;
    }

    QByteArray netlist = generateNetList();

    if(file.write( netlist ) == -1){
        file.close();
//        file.remove();
        return false;
    }

    file.close();

    // Execute
    QStringList arguments;
    arguments << "-b"  //<< QString("-o%1.out").arg(fileName)
                 << QString("%1.net").arg(fileName);

    QByteArray output;
    if(!exec( output, arguments )){
        return false;
    }

//    file.remove();

    if(!isCorrectForParse()) {
        return false;
    }

    // Parse data from file
//    QStringList columns;

    // Fill data table

    return true;
}

QByteArray NgSpiceSimulator::generateNetListModels(){
    QByteArray cards;
    circuit()->beginModel();
    SpiceModel* model = circuit()->nextModel();
    QString deviceName;
    while(model){

        switch(model->typeDevice()){
        case DEVICE_CAPACITOR:
            deviceName = "cap";
            break;
        case DEVICE_RESISTOR:
            deviceName = "res";
            break;
        case DEVICE_DIODE:
            deviceName = "d";
            break;
        case DEVICE_NBJT:
            deviceName = "NBJT";
            break;
        case DEVICE_PBJT:
            deviceName = "PBJT";
            break;
        case DEVICE_UNKNOWN:
        default:
            Q_ASSERT(false);
//            break;
        }

        cards.append( QString(".model %1 %2")
                      .arg( model->name() )
                      .arg(deviceName)
                      );
        QVariantMap parameters = model->parameters();
        if(parameters.size() > 0){
            cards.append( "(" );
            foreach( QString key, parameters.keys() ){
                cards.append( QString("%1=%2 ").arg(key)
                              .arg(parameters.value(key).toDouble()).toAscii() );
            }
            cards.append( ")" );
        }

        cards.append("\n");

        model = circuit()->nextModel();
    }
    return cards;

}

QByteArray NgSpiceSimulator::generateNetList() {
    QByteArray netlist;
    netlist.clear();

    netlist.append( QString("%1\n").arg(circuit()->name()).toAscii() );

    if(!circuit()->correct()){
        return false;
    }
    // devices
    circuit()->beginDevice();
    Device* device = circuit()->nextDevice();

    while(device){
        netlist.append( device->netList() ).append("\n");
        device = circuit()->nextDevice();
    }

    // analysis
    QByteArray source;
    switch(circuit()->typeAnalysis()){
    case ANALYSIS_DC:
        source = ".dc";
        break;
    case ANALYSIS_AC:
        source = ".ac";
        break;
    case ANALYSIS_TRAN:
        source =".tran";
        break;
    case ANALYSIS_UNKNOWN:
    default:
        Q_ASSERT(false);
    }

    circuit()->beginDevice(DEVICE_FLAG_SOURCE);
    device = circuit()->nextDevice();
    while(device){
        source.append(" ").append(device->sourceNetlist());
        device = circuit()->nextDevice();
    }
    source.append("\n");

    netlist.append(source).append( generateNetListModels() );

    return netlist;
}

bool NgSpiceSimulator::isCorrectForParse() {
    return true;
}



}
}
