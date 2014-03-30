#include <QCoreApplication>

#include "linear_search.h"
#include "QuasiNewtonExpr.h"
#include "GeneticAlgorithm.h"

double func(const VectorDouble& vectorX){
    double x1 = vectorX[0];
    double x2 = vectorX[1];
    return x1*x1-4*x1+x2*x2-10*x2+x1*x2+29;
}

VectorDouble solveGradient(const VectorDouble& vectorX){
    double x1 = vectorX[0];
    double x2 = vectorX[1];

    VectorDouble grad(2);

    grad[0] = 2*x1-4+x2;
    grad[1] = 2*x2-10+x1;

    return grad;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    VectorDouble oldX(2);
//    VectorDouble p(2);
    oldX[0] = -10;
    oldX[1] = -10;

//    QuasiNewtonExpr q;
//    VectorDouble m = q.optimize(oldX,func,solveGradient);

//    qDebug() << "End optimized";
//    qDebug() << "Fold: " << func(oldX);
//    qDebug() << "Fnew: " << func(m);

    int N=2;
    MatrixDouble constraints(N,2,MatrixDouble::MATRIX_ZERO);

    constraints.at(0,0) = 0.0;
    constraints.at(0,1) = 2.0;

    constraints.at(1,0) = 2.0;
    constraints.at(1,1) = 4.0;

    const unsigned int NUMBER_POPULATINON = std::numeric_limits<unsigned int>::max();

    GeneticAlgorithm ga;
    ga.optimize(constraints,func,NUMBER_POPULATINON);

    return 0;

}
