#include "NgSpiceSimulator.h"
#include <QFile>
#include "Circuit.h"
#include "SpiceModel.h"
#include "Device.h"
#include "Terminal.h"
#include "models/MeasureModel.h"

#include "Log.h"

namespace tsunami{
namespace spice{

NgSpiceSimulator::NgSpiceSimulator(const QString &path) :
    Simulator(path) {

}

bool NgSpiceSimulator::simulate() {
    log::logTrace() << "Simulating";
    // Append models;
    QString fileName = randomName(8);

    QFile file( QString("%1.net").arg(fileName) );
    log::logTrace() << QString("Create netlist file %1")
                       .arg(file.fileName());

    if(!file.open(QIODevice::WriteOnly)){
        log::logError() << QString("Can not open file for write %1")
                           .arg(file.fileName());
        return false;
    }

    QByteArray netlist = generateNetList();
    log::logTrace() << QString("Generated NetList\n%1").arg(QString(netlist));

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

    // No. of Data Rows :

    log::logTrace() << QString("Result simulated\n%1").arg(QString(output));

    // BULLSHIT: code
    int positionStartIndex = output.indexOf("Index");
    Q_ASSERT(positionStartIndex != -1);

    output = output.mid(positionStartIndex).replace("\r","");;

    QList<QByteArray> list =  output.split( '\n' );
    list.removeAt(0);
    list.removeAt(0);
    // Remove null items
    QList<QByteArray>::iterator it = list.begin();
    while(it!=list.end()){
        if((*it).isEmpty()){
            it = list.erase(it);
        }else{
            it++;
        }
    }

    delete simulated_;
    simulated_ = new db::MeasureModel();
    simulated_->sources( circuit()->sources() );
    simulated_->type( circuit()->typeAnalysis() );
    simulated_->attrs(QVariantMap());
    simulated_->columns(columns_);

    QVector< QVector<double> > data;

    foreach(QByteArray str,list){

        QList<QByteArray> vals = str.split('\t');
        if(vals.last().isEmpty()){
            vals.removeLast();
        }

        if( columns_.size() != vals.size() ){
            log::logDebug() << "Can not parsing "
                            << str;
            continue;
        }

        QVector<double> row;
        foreach(QByteArray val, vals){
            row.append( val.toDouble() );
        }
        data.append(row);
    }
    log::logDebug() << "Number of parsed row:"<<data.size();

    simulated_->data(data);


    return true;
}

QByteArray NgSpiceSimulator::generateNetListModels(){
    QByteArray cards;
    circuit()->beginModel();
    SpiceModel* model = circuit()->nextModel();
    while(model){
        cards.append( model->generateNetList() );
        model = circuit()->nextModel();
    }
    return cards;
}

QByteArray NgSpiceSimulator::generateNetList() {
    QByteArray netlist;
    netlist.clear();

    netlist.append( QString("%1\n\n").arg(circuit()->name()).toAscii() );

    if(!circuit()->correct()){
        return false;
    }

    netlist.append( generateNetListModels() );
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


    int sourceNumber = 1;

    while(sourceNumber < 3){
        circuit()->beginDevice(DEVICE_FLAG_SOURCE);
        device = circuit()->nextDevice();
        while(device){
            if(device->source().configuration("number").toInt() == sourceNumber){
                source.append(" ").append(device->sourceNetlist());
            }

            device = circuit()->nextDevice();
        }
        sourceNumber++;
    }
    source.append("\n");

    netlist.append(source);
    netlist.append(".width out=257\n");
    netlist.append(generateNetPrints());
    netlist.append(".options noacct nopage\n");
    netlist.append(".end\n");

    return netlist;
}

QByteArray NgSpiceSimulator::generateNetPrints() {
    QByteArray netlist = ".print ";

    columns_.clear();
    columns_ << "number" << "analysis";

    switch(circuit()->typeAnalysis()){
    case ANALYSIS_DC:
        netlist.append("dc");
        break;
    case ANALYSIS_AC:
        netlist.append("ac");
        break;
    case ANALYSIS_TRAN:
        netlist.append("tran");
        break;
    default:
        Q_ASSERT(false);
    }

//    int gndId;
//    int nPorts = device->numberPorts();
//    for(int i=0; i < nPorts; ++i){
//        device->terminal(i)->isRef();
//    }

    QList<Source> sources = circuit()->sources();

    // Looking for all input source
    circuit()->beginDevice(DEVICE_FLAG_SOURCE);
    Device* device = circuit()->nextDevice();
    while(device){
        Source source = device->source();

        Q_ASSERT( device->numberPorts() == 2 );
        Q_ASSERT( source.mode() != SOURCE_MODE_GND );

        int plus  = (!device->terminal(0)->isRef()) ? device->terminal(0)->id() : 0;
        int minus = (!device->terminal(1)->isRef()) ? device->terminal(1)->id() : 0;

        netlist.append( QString(" v(%1,%2)").arg(plus).arg(minus) );
        netlist.append( QString(" i(%1) ").arg(device->name()) );
        columns_ << QString("V%1").arg( source.node().toLower() );
        columns_ << QString("I%1").arg( source.node().toLower() );

        device = circuit()->nextDevice();
    }

    // Add ground
//    int gndId = circuit()->getRefTerminalId();
//    netlist.append( QString(" v(%1)").arg(0) ); //.arg(minus) );
//    columns_ << QString("V%1").arg( circuit()->getTerminal(gndId)->name().toLower() );

    netlist.append("\n");
    return netlist;
}

bool NgSpiceSimulator::isCorrectForParse() {
    return true;
}



}
}
