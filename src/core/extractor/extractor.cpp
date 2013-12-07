#include "extractor.h"
#include "dataset.h"
#include "spice/simulator.h"
#include "spice/circuit.h"
#include "spice/spicemodel.h"
#include "models/librarymodel.h"
#include "models/parametermodel.h"
#include "models/measuremodel.h"
#include "optimize/optimizebase.h"
#include "dbstorage/parameterstorage.h"
#include "dbstorage/settingstorage.h"
#include "spice/ngspicesimulator.h"
#include "ExtractorHookeJeeves.h"

#include <logger.h>

namespace tsunami{
namespace core{

Extractor::Extractor(DeviceType type, db::LibraryModel* library, const QList<int> &measures) : simulator_(0),
    dataset_(0),storageParameters_(0),previousFunctionError_(DBL_MAX),
    currentFunctionError_(DBL_MAX), library_(library){
    type_ = type;

    db::SettingStorage* setting = db::SettingStorage::instance();
    stopped_ = false;
    iteration_ = 0;
    maxIterations_ = setting->value("optimize/max_iteration",100).toInt();

    tolerances_.insert( TOLERANCE_FUNCTION, setting->value("optimize/tolerance/function",1e-14).toDouble()  );


    dataset_ = new Dataset();
    dataset_->load( measures );


}

db::LibraryModel *Extractor::library() {
    return library_;
}

void Extractor::simulator(spice::Simulator *simulator) {
    simulator_ = simulator;
}

void Extractor::simulator(const QString &simulator, const QString& path) {
    if(simulator.compare("ngspice") == 0){
        simulator_ = new spice::NgSpiceSimulator( path );
    }
}



int Extractor::numberParameters() {
    return library_->countParameters();
}

double Extractor::functionError() {
    double funcError = 0.0;

    Q_ASSERT(simulator_ != NULL);
//    Q_ASSERT(optimize_  != NULL);
    Q_ASSERT(dataset_   != NULL);

    dataset_->begin();
    while(dataset_->isNext()){
        const db::MeasureModel* measure = dataset_->next();
        spice::Circuit *circuit = spice::Circuit::createCircuitDevice( type_,
                                                                       measure->sources() );

        spice::SpiceModel* model = new spice::SpiceModel( "bjt" ,type_);
        model->setLibrary( library_ );


        circuit->setSpiceModel( type_, model );
        simulator_->setCircuit( circuit );
        funcError += computeError(measure);

        delete circuit;
        simulator_->setCircuit(NULL);
    }

    return funcError;
}

const QVariant &Extractor::config(const QString &key) {
    Q_ASSERT(configuration_.contains(key));

    return configuration_[key];
}

void Extractor::config(const QString &key, const QVariant &value) {
    Q_ASSERT(configuration_.contains(key));

    configuration_[key] = value;
}

void Extractor::config(const QVariantMap &config) {
    configuration_ = config;
}

const double &Extractor::fitted(int index) const {
    return library_->at(index).fitted();
}

const double &Extractor::minimum(int index) const {
    return library_->at(index).minimum();
}

const double &Extractor::maximum(int index) const {
    return library_->at(index).maximum();
}

const bool &Extractor::fixed(int index) const {
    return library_->at(index).fixed();
}

void Extractor::fitted(int index, double fitted) {
    library()->at(index).fitted( fitted );
}

double Extractor::step(int index) const {
    return currentSteps_[index];
}

void Extractor::step(int index, double step) {
    currentSteps_[index] = step;
}

Extractor *Extractor::createExtractor(const QString &methodOptimization, DeviceType type,
                                      db::LibraryModel* library, const QList<int> &measures) {
    if(methodOptimization.compare("hookejeeves") == 0){
        return new ExtractorHookeJeeves(type,library,measures);
    }


    return NULL;
}

void Extractor::stop() {
    stopped_ = true;
}

bool Extractor::testBoundary(int index, double value) {
    double val = fabs(value);
    return (fabs(minimum(index)) >= val && val <= fabs(maximum(index)));
}

void Extractor::increaseIteration() {
    iteration_++;
}

bool Extractor::checkConvergence(bool showMessage) {

    if(stopped_){
        return false;
    }

    if( tolerances_.contains(TOLERANCE_FUNCTION) ){
        double tol = tolerances_[TOLERANCE_FUNCTION];
        qDebug() << previousFunctionError_ - currentFunctionError_;
        if( fabs(previousFunctionError_ - currentFunctionError_) < tol ){
            if(showMessage) emit log("- Detected function tolerance condition");
            return false;
        }

    }

    if( tolerances_.contains(TOLERANCE_STEP)){
        double tol = tolerances_[TOLERANCE_STEP];
        int nParameters = numberParameters();
        for(int i=0; i < nParameters; ++i){
            if( !library_->parameter(i).enable() || library_->parameter(i).fixed() ){
                continue;
            }
            if(fabs(previousSteps_[i] - currentSteps_[i]) < tol){
               if(showMessage) emit log("- Detected step tolerance condition");
               return false;
            }
        }

    }
    if( tolerances_.contains(TOLERANCE_GRADIENT)){
        Q_ASSERT(false);
    }

    if( maxIterations_ < iteration_ ){
        return false;
    }

    return true;

}

void Extractor::currentFunctionError(double funcError) {
    previousFunctionError_ = currentFunctionError_;
    currentFunctionError_ = funcError;
}

const double &Extractor::currentFunctionError() {
    return currentFunctionError_;
}

void Extractor::saveSteps() {
    previousSteps_ = currentSteps_;
}

double Extractor::subtract(double value1, double value2) {
    double subtract = value2 - value1;
    return subtract;
}

double Extractor::computeError(const db::MeasureModel *measure) {
    simulator_->simulate();
    db::MeasureModel* simulate = simulator_->simulatedData();
    int rows = measure->rows();
    double error;

    int nNotFound = 0;
    for(int i=0; i < rows; ++i){
        QMap<QString, double> measured  = measure->get(i);
        QMap<QString, double> simulated = simulate->find(measured);

        if(simulated.size() == 0){
//            log::logTraceF("Not found simulated data for %s",measured);
            nNotFound++;
            continue;
        }


        foreach(Source source, measure->sources()){
            if(source.direction() == SOURCE_DIRECTION_OUTPUT){
                error += fabs( subtract( measured.value(source.name(),.0),
                                         simulated.value(source.name(),.0))  );
            }
        }

    }



//    log::logDebugF("Number found: %d",rows-nNotFound);



    return error;
}


}
}
