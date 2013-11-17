#include "extractorglobal.h"

namespace tsunami{
namespace core{


ExtractorGlobal::ExtractorGlobal() {
}

double ExtractorGlobal::computeError(db::MeasureModel *measure) {

    // TODO Add PRINTS
    simulator_->simulate();
    db::MeasureModel simulate = simulator_->simulatedData();
    int rows = measure->rows();
    double error;

    for(int i=0; i < rows; ++i){
        QMap<QString, double> measured  = measure->get(i);
        QMap<QString, double> simulated = simulate.find(measured);


        if(type_ == DEVICE_PBJT || type_ == DEVICE_NBJT){
            error += subDataValue(simulated.value("Ie"),measured.value("Ie"));
            error += subDataValue(simulated.value("Ib"),measured.value("Ib"));
            error += subDataValue(simulated.value("Ic"),measured.value("Ic"));
        }
    }



//    delete model;
//    delete circuit;



}



}
}
