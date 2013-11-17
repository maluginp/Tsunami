#include "extractor.h"
#include "dataset.h"
#include "spice/simulator.h"
#include "models/librarymodel.h"
#include "models/parametermodel.h"
#include "optimize/optimizebase.h"

namespace tsunami{
namespace core{

Extractor::Extractor() {
}

void Extractor::run() {
    optimize_->setExtractor(this);
    optimize_->run();
}


double Extractor::functionError() {
    double funcError = 0.0;
    dataset()->begin();

    while(dataset->isNext()){
        db::MeasureModel measure = dataset()->next();
        spice::Circuit *circuit = spice::Circuit::createCircuitDevice( type_,
                                                                       measure.sources() );

        spice::SpiceModel* model = new spice::SpiceModel("bjt_model");
        foreach(db::ParameterModel parameter, parameters_){
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


}
}
