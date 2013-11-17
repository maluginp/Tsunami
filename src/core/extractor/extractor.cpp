#include "extractor.h"
#include "dataset.h"
#include "spice/simulator.h"
#include "models/librarymodel.h"
#include "models/parametermodel.h"
namespace tsunami{
namespace core{

Extractor::Extractor() {
}


double Extractor::functionError() {
    double funcError = 0.0;
    simulator_->updateParameters( library_.name(), parameters_ );

    simulator_->simulate();

    db::MeasureModel simulated = simulator_->simulatedData();

    dataset()->begin();

    while(dataset->isNext()){
        db::MeasureModel measure = dataset()->next();
        funcError += computeError( &simulated, &measure );
    }

    return funcError;
}


}
}
