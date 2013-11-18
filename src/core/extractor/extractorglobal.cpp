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
        }else if(type_ == DEVICE_NMOS || type_ == DEVICE_PMOS
                 || type_ == DEVICE_NFET || type_ == DEVICE_PFET){
            error += subDataValue(simulated.value("Is"),measured.value("Is"));
            error += subDataValue(simulated.value("Ig"),measured.value("Ig"));
            error += subDataValue(simulated.value("Id"),measured.value("Id"));
            error += subDataValue(simulated.value("Ib"),measured.value("Ib"));

        }
    }



//    delete model;
//    delete circuit;

    return error;

}



}
}
