#include "optimizebase.h"
#include <float.h>
#include <QString>
#include "../math/matrix.h"
#include "extractor/extractor.h"
namespace tsunami{
namespace core{

OptimizeBase::OptimizeBase(Extractor *extractor, int maxIteration)
    : extractor_(extractor)
    , maxIteration_(maxIteration)
    , iteration_(0) {

}

Extractor *OptimizeBase::extractor() {
    return extractor_;
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

int OptimizeBase::countParameters() {
    return extractor()->countParameters();
}

double OptimizeBase::functionError() {
    if(extractor()){
        return extractor()->functionError();
    }

    return DBL_MAX;
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

const double &OptimizeBase::lastFunctionError() {
    return lastFunctionError_;
}

void OptimizeBase::lastFunctionError(double error) {
    prevFunctionError_ = lastFunctionError_;
    lastFunctionError_ = error;
}

void OptimizeBase::saveGradient(const MatrixDouble &gradient) {
    if(lastGradient_.isEmpty()){
        lastGradient_ = gradient;
    }else{
        lastGradient_ = currentGradient_;
    }

    currentGradient_ = gradient;
}

void OptimizeBase::saveHessian(const MatrixDouble &hessian) {
    Q_ASSERT(false);
//    if(lastHessian_.isVector()){
//        lastHessian_ = hessian;
//    }else{
//        lastHessian_ = currentHessian_;
//    }

//    currentHessian_ = hessian;
}


}
}
