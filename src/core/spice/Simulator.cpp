#include "Simulator.h"
#include <QProcess>
#include "models/MeasureModel.h"
#include "Log.h"
#include "spice/Circuit.h"

#include "Device.h"
#include "Devices.h"

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

void Simulator::analyses(const QVariantList &analyses) {
    analyses_ = analyses;
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

void Simulator::typeAnalysis(AnalysisType analysisType) {
    typeAnalysis_ = analysisType;
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
    nets.append( QString("%1\n\n").arg(cir->name()) );

    qDebug() << nets;
//    // Сгенерируем все net для устройств
    foreach(Device* device, cir->getDevices()){
        nets.append( QString("%1\n").arg(QString(device->netlist())) );
    }

    QByteArray modelCards = netListModels();
    nets.append( modelCards );

    nets.append( netListAnalyses() );
    nets.append( "\n.width out=257\n" );
    nets.append( netListPrints() );
    nets.append(".options noacct nopage\n");
    nets.append(".end\n");
    log::logTrace() << QString("Generated NetList\n%1")
                       .arg(QString(nets));
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

    foreach(Device* device,circuit_->getDevices(DEVICE_FLAG_HAVE_MODEL)){
        if(device->hasSpiceModel()){
            cards.append( device->spiceModel()->generateNetList() );
        }
    }

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

    columns_.clear();
    columns_ << "number" << "analysis";

    switch(typeAnalysis_){
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


    foreach(Device* device,circuit()->getDevices( DEVICE_FLAG_SOURCE )){
        IndependentSourceDevice* sourceDevice = dynamic_cast<IndependentSourceDevice*>(device);
//        QStringList terminals = sourceDevice->terminals();

        if(sourceDevice->type() == DEVICE_VSOURCE){
//            columns_.append(sourceDevice->name());
            QString column = sourceDevice->name();
            column[0] = 'I';
            columns_.append(column);
            netlist.append(
                        QString(" i(%1)")
//                           .arg(sourceDevice->terminal("P")->id())
//                           .arg(sourceDevice->terminal("M")->id())
                           .arg(sourceDevice->name())
                        );

        }else if(sourceDevice->type() == DEVICE_ISOURCE){
//            columns_.append(sourceDevice->name());
            QString column = sourceDevice->name();
            column[0] = 'V';
            columns_.append(column);

            netlist.append(
                        QString(" v(%1,%2)")
//                           .arg(sourceDevice->name())
                           .arg(sourceDevice->terminal("P")->id())
                           .arg(sourceDevice->terminal("M")->id())
                        );
        }
    }

    netlist.append("\n");
    return netlist;


}

QByteArray Simulator::netListAnalyses() {
    QString source;
    QVariantMap analysis = analyses_[0].toMap();

    if(typeAnalysis_ == ANALYSIS_DC){
        QString name = analysis.value("name").toString();
        double start = analysis.value("start").toDouble();
        double step = analysis.value("step").toDouble();
        double stop  = analysis.value("stop").toDouble();

        source = QString(".dc %1 %2 %3 %4").arg(name).arg(start).arg(stop).arg(step);

        if(analyses_.count() == 2){
            QVariantMap analysisSecond = analyses_[1].toMap();
            QString nameSecond = analysisSecond.value("name").toString();
            double startSecond = analysisSecond.value("start").toDouble();
            double stepSecond = analysisSecond.value("step").toDouble();
            double stopSecond  = analysisSecond.value("stop").toDouble();

            source.append(QString(" %1 %2 %3 %4").arg(nameSecond)
                          .arg(startSecond)
                          .arg(stopSecond)
                          .arg(stepSecond));

        }

    }else{
        if(typeAnalysis_ == ANALYSIS_AC){
            int points = analysis.value("points").toInt();
            double start = analysis.value("start").toDouble();
            double stop  = analysis.value("stop").toDouble();
            // dec, oct, lin
            QString variation = analysis.value("variation").toString();

            source = QString(".ac %1 %2 %3 %5").arg(variation)
                    .arg(points).arg(start).arg(stop);
        }else if(typeAnalysis_ == ANALYSIS_TRAN){

            double start = analysis.value("start").toDouble();
            double step = analysis.value("step").toDouble();
            double stop  = analysis.value("stop").toDouble();

            source = QString(".tran %1 %2 %3").arg(step).arg(stop).arg(start);
        }
    }



    return source.toAscii();
}



}
}
