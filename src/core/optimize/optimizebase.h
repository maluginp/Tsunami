#ifndef OPTIMIZEBASE_H
#define OPTIMIZEBASE_H
#include <QMap>
#include "defines.h"

namespace tsunami{
namespace core{

class Extractor;

class OptimizeBase {
public:
    enum TypeTolerance{
        TOLERANCE_UNKNOWN,
        TOLERANCE_STEP,
        TOLERANCE_FUNCTION,
        TOLERANCE_GRADIENT
    };

    OptimizeBase(Extractor* extractor, int maxIteration=255);
    virtual void run() = 0;
    Extractor* extractor();

    void setEps( double eps );
    const double& eps();

    void setExtractor( Extractor* extractor );
//    void setStepWeight( const QString& name, double weight );
//    double stepWeight( const QString& name);

    void setTolerance( TypeTolerance tol, double value );
    const double& tolerance( TypeTolerance tol );

    void setStep(const QString& param, double step);
    const double& step( const QString& param);
    const double& step( int index);
    void step(int index, double step);


    void value(int index, double value);
    double value(int index);
    bool isFixed( int index );

    int countParameters();
    void saveSteps();

protected:
    virtual double functionError();

    bool checkConvergence();
    double computeError();

    void nextIteration();

    const double& lastFunctionError();
    void lastFunctionError(double error);


    void saveGradient( const MatrixDouble& gradient );
    void saveHessian( const MatrixDouble& hessian);
    void saveFunctionError( double functionError );

private:
    int maxIteration_;

    int iteration_;
    QMap<int,double> steps_;
    QMap<int,double> prevSteps_;
    double eps_;
    double toleranceStep_;
    double toleranceFunction_;
    double toleranceGradient_;

    MatrixDouble lastGradient_, currentGradient_;
//    MatrixDouble lastHessian_,  currentHessian_;
    double lastFunctionError_, currentFunctionError_;


    Extractor* extractor_;
};

}
}


#endif // OPTIMIZEBASE_H
