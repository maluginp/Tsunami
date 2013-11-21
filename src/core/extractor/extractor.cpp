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

// Optimize methods
#include "optimize/hookejeevesmethod.h"

namespace tsunami{
namespace core{

Extractor::Extractor(DeviceType type, int libraryId) {
    type_ = type;

    storage_ = db::ParameterStorage::instance();
    library_ = storage_->openLibrary( libraryId );


}

void Extractor::setDataset(Dataset *dataset) {
    dataset_ = dataset;
}

void Extractor::setMethodOptimize(OptimizeBase *optimize) {
    optimize_ = optimize;
}

void Extractor::setMethodOptimize(const QString &name) {
    if(name.compare("hookejeeves", Qt::CaseInsensitive)){
        optimize_ = new HookeJeevesMethod(this);
    }

}

void Extractor::setSimulator(spice::Simulator *simulator) {
    simulator_ = simulator;
}

const double &Extractor::value(const QString &name) const {
    return library_->parameter( name ).fitted();
}

const double &Extractor::value(int index) const {
    return library_->parameter( index ).fitted();
}

const double &Extractor::initial(const QString &name) const {
    return library_->parameter(name).initial();
}

const double &Extractor::initial(int index) const {
    return library_->parameter(index).initial();
}

const double &Extractor::minimum(const QString &name) const {
    return library_->parameter(name).minimum();
}

const double &Extractor::minimum(int index) const {
    return library_->parameter(index).minimum();
}

const double &Extractor::maximum(const QString &name) const {
    return library_->parameter(name).maximum();
}

const double &Extractor::maximum(int index) const {
    return library_->parameter(index).maximum();
}

const double &Extractor::fixed(const QString &name) const {
    return library_->parameter(name).fixed();
}

const double &Extractor::fixed(int index) const {
    return library_->parameter(index).fixed();
}

const double &Extractor::enable(const QString &name) const {
    return library_->parameter(name).enable();
}

const double &Extractor::enable(int index) const {
    return library_->parameter(index).enable();
}

void Extractor::value(const QString &name, double value) {
    library_->parameter(name).fitted(value);
}

void Extractor::value(int index, double value) {
    library_->parameter(index).fitted(value);
}

void Extractor::initial(const QString &name, double initial) {
    library_->parameter(name).initial(initial);
}

void Extractor::initial(int index, double initial){
    library_->parameter(index).initial(initial);
}

void Extractor::minimum(const QString &name, double minimum) {
    library_->parameter(name).minimum(minimum);
}

void Extractor::minimum(int index, double minimum) {
    library_->parameter(index).minimum(minimum);
}

void Extractor::maximum(const QString &name, double maximum) {
    library_->parameter(name).maximum(maximum);
}

void Extractor::maximum(int index, double maximum) {
    library_->parameter(index).maximum(maximum);
}

void Extractor::fixed(const QString &name, bool fixed) {
    library_->parameter(name).fixed(fixed);
}

void Extractor::fixed(int index, bool fixed) {
    library_->parameter(index).fixed(fixed);
}

void Extractor::enable(const QString &name, bool enable) {
    library_->parameter(name).enable(enable);
}

void Extractor::enable(int index, bool enable) {
    library_->parameter(index).enable(enable);
}

int Extractor::countParameters() {
    return library_->countParamaters();

}

void Extractor::run() {
    Q_ASSERT(optimize_ != NULL);

    // Go to Mars
    optimize_->run();


}


double Extractor::functionError() {
    double funcError = 0.0;

    Q_ASSERT(simulator_ != NULL);
    Q_ASSERT(optimize_  != NULL);
    Q_ASSERT(dataset_   != NULL);

    dataset()->begin();
    while(dataset()->isNext()){
        db::MeasureModel measure = dataset()->next();
        spice::Circuit *circuit = spice::Circuit::createCircuitDevice( type_,
                                                                       measure.sources() );

        spice::SpiceModel* model = new spice::SpiceModel("device");
        foreach(db::ParameterModel parameter, library_->parameters()){
            model->add( parameter.name(), parameter.fitted() );
        }

        circuit->setSpiceModel( type_, model );
        simulator_->setCircuit( circuit );
        funcError += computeError(&measure);

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

double Extractor::subtract(double value1, double value2) {
    double subtract = value2 - value1;
    return subtract;
}


}
}
