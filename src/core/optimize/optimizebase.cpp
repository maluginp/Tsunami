#include "optimizebase.h"
#include <float.h>
#include <QString>
#include "../math/matrix.h"

namespace tsunami{
namespace core{

OptimizeBase::OptimizeBase(Simulator *simulator, int maxIteration)
    : simulator_(simulator)
    , maxIteration_(maxIteration)
    , iteration_(0) {

}

Simulator *OptimizeBase::simulator() {
    return simulator_;
}

void OptimizeBase::setEps(double eps) {
    eps_ = eps;
}

const double &OptimizeBase::eps() {
    return eps_;
}

void OptimizeBase::setTolerance(OptimizeBase::TypeTolerance tol, double value) {
    switch(tol){
    case TOLERANCE_FUNCTION:
        toleranceFunction_ = value; break;
    case TOLERANCE_GRADIENT:
        toleranceGradient_ = value; break;
    case TOLERANCE_STEP:
        toleranceStep_     = value; break;
    case TOLERANCE_UNKNOWN:
    default:
        break;
    }
}

const double &OptimizeBase::tolerance(OptimizeBase::TypeTolerance tol) {
    switch(tol){
    case TOLERANCE_FUNCTION:
        return toleranceFunction_;
    case TOLERANCE_GRADIENT:
        return toleranceGradient_;
    case TOLERANCE_STEP:
        return toleranceStep_;
    case TOLERANCE_UNKNOWN:
    default:
        break;
    }
    Q_ASSERT(false);
    return 0.0;

}

void OptimizeBase::setStep(const QString &param, double step) {
    steps_.insert( param, step );
}

const double &OptimizeBase::step(const QString &param) {
    return steps_[param];
}

bool OptimizeBase::checkConvergence() {
    return false;
}

double OptimizeBase::computeError() {
    return DBL_MAX;
}

void OptimizeBase::nextIteration() {
    iteration_++;

}

//void OptimizeBase::saveGradient(const MatrixDouble &gradient) {
//    if(lastGradient_.isEmpty()){
//        lastGradient_ = gradient;
//    }else{
//        lastGradient_ = currentGradient_;
//    }

//    currentGradient_ = gradient;
//}

//void OptimizeBase::saveHessian(const MatrixDouble &hessian)
//{
//    if(lastHessian_.isVector()){
//        lastHessian_ = hessian;
//    }else{
//        lastHessian_ = currentHessian_;
//    }

//    currentHessian_ = hessian;
//}

//void OptimizeBase::saveFunctionError(double functionError) {
//    if(lastFunctionError_ == DBL_MAX){

//    }
//}

}
}
