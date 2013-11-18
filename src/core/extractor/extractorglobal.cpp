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
            error += subDataValue(measured.value("Ie"),simulated.value("Ie"));
            error += subDataValue(measured.value("Ib"),simulated.value("Ib"));
            error += subDataValue(measured.value("Ic"),simulated.value("Ic"));
        }else if(type_ == DEVICE_NMOS || type_ == DEVICE_PMOS
                 || type_ == DEVICE_NFET || type_ == DEVICE_PFET){
            error += subDataValue(measured.value("Is"),simulated.value("Is"));
            error += subDataValue(measured.value("Ig"),simulated.value("Ig"));
            error += subDataValue(measured.value("Id"),simulated.value("Id"));
            error += subDataValue(measured.value("Ib"),simulated.value("Ib"));
        }
    }



//    delete model;
//    delete circuit;

    return error;

}



}
}
