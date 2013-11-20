#ifndef HOOKEJEEVESMETHOD_H
#define HOOKEJEEVESMETHOD_H

#include "optimizebase.h"

namespace tsunami{
namespace core{

class Extractor;

class HookeJeevesMethod : public OptimizeBase {
public:
    enum Masks{
        HJ_DEC,
        HJ_INC,
        HJ_HOLD
    };

    HookeJeevesMethod(Extractor* extractor);
    void run();
private:
    const double& tempFunctionError();
    void tempFunctionError(double error);


    void findBestNearby();
    void patternStep();
    void decreaseSteps();

    void mask(int index, Masks mask);
    Masks mask(int index);

    QMap<int,Masks> masks_;


};

}
}


#endif // HOOKEJEEVESMETHOD_H
