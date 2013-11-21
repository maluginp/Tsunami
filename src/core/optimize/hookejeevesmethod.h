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
    const double& tempFunctionError() { return tempFunctionError_; }
    void tempFunctionError(double error) { tempFunctionError_ = error; }


    bool findBestNearby();
    void patternStep();
    bool decreaseSteps();

    void mask(int index, Masks mask);
    Masks mask(int index);

    QMap<int,Masks> masks_;
    double tempFunctionError_;

};

}
}


#endif // HOOKEJEEVESMETHOD_H
