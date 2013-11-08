#ifndef OPTIMIZEBASE_H
#define OPTIMIZEBASE_H
#include <QMap>
#include "defines.h"

namespace tsunami{
namespace core{

class Simulator;

class OptimizeBase {
public:
    enum TypeTolerance{
        TOLERANCE_UNKNOWN,
        TOLERANCE_STEP,
        TOLERANCE_FUNCTION,
        TOLERANCE_GRADIENT
    };

    OptimizeBase(Simulator* simulator, int maxIteration);
    virtual void run() = 0;
    Simulator* simulator();

    void setEps( double eps );
    const double& eps();

//    void setStepWeight( const QString& name, double weight );
//    double stepWeight( const QString& name);


    void setTolerance( TypeTolerance tol, double value );
    const double& tolerance( TypeTolerance tol );

    void setStep(const QString& param, double step);
    const double& step( const QString& param);

protected:
    bool checkConvergence();
    double computeError();

    void nextIteration();

    void saveGradient( const MatrixDouble& gradient );
    void saveHessian( const MatrixDouble& hessian);
    void saveFunctionError( double functionError );



private:
    int maxIteration_;

    int iteration_;
    QMap<QString,double> steps_;
//    QMap<QString,double> stepWeights_;
    double eps_;
    double toleranceStep_;
    double toleranceFunction_;
    double toleranceGradient_;

    MatrixDouble lastGradient_, currentGradient_;
    MatrixDouble lastHessian_,  currentHessian_;
    double lastFunctionError_, currentFunctionError_;


    Simulator* simulator_;
};

}
}


#endif // OPTIMIZEBASE_H
