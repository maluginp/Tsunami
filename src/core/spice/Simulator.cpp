#include "Simulator.h"
#include <QProcess>
#include "models/MeasureModel.h"
#include "Log.h"
#include "spice/Circuit.h"
namespace tsunami{
namespace spice{


Simulator::Simulator()
    : external_(false)
    , circuit_(NULL), simulated_(NULL) {
    Q_ASSERT(false);
}

Simulator::Simulator(const QString &path)
    : external_(true)
    , circuit_(NULL)
    , path_(path), simulated_(NULL)
{


}

void Simulator::setCircuit(Circuit *circuit) {
    if(circuit != NULL){
        circuit_ = circuit;
    }
}

Circuit *Simulator::circuit() {
    Q_ASSERT(circuit_ != NULL);
    return circuit_;
}

const bool &Simulator::external() const {
    return external_;
}

const QString &Simulator::path() const {
    return path_;
}

void Simulator::external(bool external) {
    external_ = external;
}

void Simulator::path(const QString &path) {
    path_ = path;
}

db::MeasureModel* Simulator::simulatedData(){
    return simulated_;
}

bool Simulator::exec(QByteArray& data, const QStringList &arguments){
//    log::logTrace() << "Execute simulator";
    QProcess process;
    process.setProcessChannelMode( QProcess::MergedChannels );
    // Start the process
    process.start(path(), arguments ,QIODevice::ReadWrite);
//    log::logTrace() << "Start" << path() << " with " << arguments;

    data.clear();
    // Wait for it to start
    if(!process.waitForStarted()){
        log::logError() << "Process isn't started";
        return false;
    }

    // Continue reading the data until EOF reached
    while(process.waitForReadyRead()){
        data.append(process.readAll());
    }

//    log::logTrace() << "Data parsed. Count bytes:" << data.count();

    return true;
}

QByteArray Simulator::netlist() {
    Q_ASSERT(circuit_);

    Circuit* cir = circuit_;

    QByteArray nets;
//    nets.append( QString("%1\n\n").arg(cir->name()) );

//    // Сгенерируем все net для устройств
//    cir->beginDevice();
//    Device* device = cir->nextDevice();
//    while(device){
//        nets.append( QString("%1\n").arg(device->netList()) );
//    }

//    QByteArray modelCards = netListModels();
//    nets.append( modelCards );

//    nets.append( netListAnalysis() );
//    nets.append( ".width out=257\n" );
//    nets.append( netListPrints() );
//    nets.append(".options noacct nopage\n");
//    nets.append(".end\n");
//    log::logTrace() << QString("Generated NetList\n%1")
//                       .arg(QString(nets));
    return nets;
}

QString Simulator::randomName(int num) {
    // FIXME: implementation random name generator for netlist
    return QString("Mock%1").arg(num);
}
// \todo Стоит подумать о том чтобы убрать класс SpiceModel,
// а им заменить db::LibraryModel
QByteArray Simulator::netListModels() {
    QByteArray cards;
//    circuit_->beginModel();
//    SpiceModel* model = circuit_->nextModel();
//    while(model){
//        cards.append( model->toNetList() );
//        model = circuit_->nextModel();
//    }
    return cards;
}

QByteArray Simulator::netListPrints() {
    QByteArray netlist = ".print ";

//    columns_.clear();
//    columns_ << "number" << "analysis";

//    switch(circuit()->typeAnalysis()){
//    case ANALYSIS_DC:
//        netlist.append("dc");
//        break;
//    case ANALYSIS_AC:
//        netlist.append("ac");
//        break;
//    case ANALYSIS_TRAN:
//        netlist.append("tran");
//        break;
//    default:
//        Q_ASSERT(false);
//    }



//    QList<Source> sources = circuit()->sources();

//    // Looking for all input source
//    circuit()->beginDevice(DEVICE_FLAG_SOURCE);
//    Device* device = circuit()->nextDevice();
//    while(device){
//        Source source = device->source();

//        Q_ASSERT( device->numberPorts() == 2 );
//        Q_ASSERT( source.mode() != SOURCE_MODE_GND );

//        int plus  = (!device->terminal(0)->isRef()) ? device->terminal(0)->id() : 0;
//        int minus = (!device->terminal(1)->isRef()) ? device->terminal(1)->id() : 0;

//        if( device->type() == DEVICE_VSOURCE ){
//            netlist.append( QString(" v(%1,%2)").arg(plus).arg(minus) );
//            netlist.append( QString(" i(%1) ").arg(device->name()) );
//            columns_ << QString("V%1").arg( source.node().toLower() );

//            columns_ << QString("I%1").arg( source.node().toLower() );

//            if(!source.isPositive()){
//                changeSigns_ << columns_.last();
//            }
//        }else{
//            Q_ASSERT(false);
//        }
//        device = circuit()->nextDevice();
//    }

    // Add ground
//    int gndId = circuit()->getRefTerminalId();
//    netlist.append( QString(" v(%1)").arg(0) ); //.arg(minus) );
//    columns_ << QString("V%1").arg( circuit()->getTerminal(gndId)->name().toLower() );

    netlist.append("\n");
    return netlist;


}

QByteArray Simulator::netListAnalysis() {
    QByteArray source;
    Circuit* cir = circuit_;
//    switch(cir->typeAnalysis()){
//    case ANALYSIS_DC:
//        source = ".dc";
//        break;
//    case ANALYSIS_AC:
//        source = ".ac";
//        break;
//    case ANALYSIS_TRAN:
//        source =".tran";
//        break;
//    case ANALYSIS_UNKNOWN:
//    default:
//        Q_ASSERT(false);
//    }

    // \todo Source to netlist for analysis

//    int sourceNumber = 1;

//    while(sourceNumber <= 2){
//        cir->beginDevice(DEVICE_FLAG_SOURCE);
//        device = cir->nextDevice();
//        while(device){
//            if(device->source().configuration("number").toInt() == sourceNumber){
//                source.append(" ").append(device->sourceNetlist());
//            }

//            device = circuit()->nextDevice();
//        }
//        sourceNumber++;
//    }
//    source.append("\n");

    return source;
}



}
}
