#include "Extractor.h"
#include "Dataset.h"
#include "spice/Simulator.h"
#include "spice/Circuit.h"
#include "spice/SpiceModel.h"
#include "dbstorage/DbStorages.h"
#include "spice/NgSpiceSimulator.h"
#include "ExtractorHookeJeeves.h"

#include <Log.h>
namespace tsunami{
using namespace spice;

namespace core{



Extractor::Extractor(DeviceType type, db::LibraryModel* library, const QList<int> &measures) :
        previousFunctionError_(DBL_MAX),
        currentFunctionError_(DBL_MAX),
        simulator_(0),
        library_(library),
        dataset_(0),
        storageParameters_(0)
{
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
    Q_ASSERT(dataset_   != NULL);

    dataset_->begin();
    while(dataset_->isNext()){
        const db::MeasureModel* measure = dataset_->next();
        Circuit* circuit = new Circuit("Check function error");

        simulator_->setCircuit(circuit);
        simulator_->analyses( measure->analyses() );
        simulator_->typeAnalysis( measure->type() );
        simulator_->measure(measure);
        SpiceModel* model = new SpiceModel("SPICEMODEL",type_);
        circuit->create( type_, measure->sources(), model);
        model->setLibrary(library_);

        funcError += computeError(measure);

        if(funcError == TSUNAMI_DOUBLE_MAX || funcError == TSUNAMI_DOUBLE_INF){
            return TSUNAMI_DOUBLE_MAX;
        }

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

QString Extractor::nameParameter(int index) const {
    return library_->at(index).name();

}

const bool &Extractor::enable(int index) const{
    return library_->at(index).enable();
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

QString Extractor::debugSteps() {
    QString dbg;


    dbg = "Param\t|Step\t|\n";
    foreach(int index, currentSteps_.keys()){
        if(enable(index) && !fixed(index)){
            dbg.append( QString("%1\t|%2\t|\n")
                    .arg(nameParameter(index))
                    .arg(currentSteps_[index]) );

        }
    }

    return dbg;

}

bool Extractor::testBoundary(int index, double value) {
//    double val = fabs(value);
//    return (fabs(minimum(index)) >= val && val <= fabs(maximum(index)));
    return (minimum(index) >= value && value <= maximum(index));
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
//        qDebug() << previousFunctionError_ - currentFunctionError_;
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
    if(simulator_->simulate()){
        db::MeasureModel* simulate = simulator_->simulatedData();
        int rows = measure->rows();
        double error;

        int nNotFound = 0;
        for(int i=0; i < rows; ++i){
            QMap<QString, double> measured  = measure->get(i);
            QMap<QString, double> simulated = simulate->find(measured);

            if(simulated.size() == 0){
                nNotFound++;
                continue;
            }

            foreach(Source* source, measure->sources()){
                if(source->direction() == SOURCE_DIRECTION_OUTPUT){
                    error += fabs( subtract( measured.value(source->name(),.0),
                                             simulated.value(source->name(),.0))  );
                }
                /// @note Нет смысла дальше продолжать расчёт функции ошибки
                if(error == TSUNAMI_DOUBLE_INF || error == TSUNAMI_DOUBLE_MAX){
                    return TSUNAMI_DOUBLE_MAX;
                }
            }

        }
        return error;
    }


    return TSUNAMI_DOUBLE_MAX;
}


}
}
