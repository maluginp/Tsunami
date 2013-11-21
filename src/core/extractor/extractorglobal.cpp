#include "extractorglobal.h"
#include "spice/simulator.h"
#include "models/measuremodel.h"

namespace tsunami{
namespace core{


ExtractorGlobal::ExtractorGlobal(DeviceType type, int libraryId) : Extractor(type,libraryId) {
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
            error += subtract(measured.value("Ie"),simulated.value("Ie"));
            error += subtract(measured.value("Ib"),simulated.value("Ib"));
            error += subtract(measured.value("Ic"),simulated.value("Ic"));
        }else if(type_ == DEVICE_NMOS || type_ == DEVICE_PMOS
                 || type_ == DEVICE_NFET || type_ == DEVICE_PFET){
            error += subtract(measured.value("Is"),simulated.value("Is"));
            error += subtract(measured.value("Ig"),simulated.value("Ig"));
            error += subtract(measured.value("Id"),simulated.value("Id"));
            error += subtract(measured.value("Ib"),simulated.value("Ib"));
        }
    }



//    delete model;
//    delete circuit;

    return error;

}



}
}
