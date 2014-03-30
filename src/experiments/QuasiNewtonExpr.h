#ifndef QUASINEWTONEXPR_H
#define QUASINEWTONEXPR_H
#include "defines.h"
using namespace tsunami;

class QuasiNewtonExpr
{
public:
    QuasiNewtonExpr();
    VectorDouble optimize(const VectorDouble& vectorX, FUNCTION func, FUNC_GRADIENT grad);

private:

    VectorDouble linSearch(FUNCTION func, const VectorDouble& oldX,
                           const VectorDouble &gradient,
                           VectorDouble &S, double &lambda);


    void print(const MatrixDouble& matrix);
    void print(const VectorDouble& vector);
    MatrixDouble solveAk(const MatrixDouble& Ak,
                         const VectorDouble& dGradient,
                         const VectorDouble& dX);
    VectorDouble modifyS(const VectorDouble& S,
                         double cappa);
};

#endif // QUASINEWTONEXPR_H
