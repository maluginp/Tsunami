#include "ExtractorQuasiNewton.h"


namespace tsunami{
namespace core{

ExtractorQuasiNewton::ExtractorQuasiNewton(DeviceType type,
                                           db::LibraryModel *library,
                                           const QList<int> &measures)
    : Extractor(type,library,measures)
{
    int N = numberParameters();

    Matrix<double> Ak(N,N,Matrix::MATRIX_ZERO),
            Ak2(N,N,Matrix::MATRIX_ZERO),
//            Xk1(N,1,Matrix::MATRIX_ZERO),
//            Xk(N,1,Matrix::MATRIX_ZERO),
            S(N,1,Matrix::MATRIX_ZERO),
//            Grad(N,1,Matrix::MATRIX_ZERO),
//            LastGrad(N,1,Matrix::MATRIX_ZERO),
            dX(N,1,Matrix::MATRIX_ZERO),
            dGrad(N,1,Matrix::MATRIX_ZERO);






}

void ExtractorQuasiNewton::process() {
    int N = numberParameters();

    Vector<double> gradient = solveGradient();
    Vector<double> lastGradient(N), dGradient(N);

    currentFunctionError_ = functionError();

    Matrix<double> Ak1(N,N,Matrix::MATRIX_IDENTITY),
            Ak(N,N,Matrix::MATRIX_ZERO);

    Vector<double> S(N);

    Vector<double> Xk(N), Xk1(N), dX(N);


    for(int i=0; i < N; ++i){
        Xk[i] = fitted(i);
    }

    Xk1 = Xk;

    double cappa = .0;

    while(checkConvergence()){
        increaseIteration();
        Xk = Xk1;

        // Одномерная оптимизация
        cappa = .0;

        // Выполняем преобразования
        Vector<double> modifiedS = modifyS(S,cappa);

        Xk1 = Xk + modifiedS;

        lastGradient = gradient;
        gradient = solveGradient();

        dGradient = gradient - lastGradient;
        dX = Xk1 - Xk;

        Ak = Ak1;

        Ak1 = solveAk(Ak);



    }
}

Vector<double> ExtractorQuasiNewton::modifyS(const Vector<double> &S, double cappa) {
    int N = numberParameters();
    Vector<double> modifiedS(N);

    for(int i=0; i < N; ++i){
        modifiedS[i] = S[i]*cappa;
    }

    return modifiedS;
}

Matrix<double> ExtractorQuasiNewton::solveAk(const Matrix<double> &Ak,
                                             const Vector<double>& dGradient,
                                             const Vector<double>& dX) {
    Matrix<double> solvedAk(Ak,Matrix::MATRIX_COPY);

    solvedAk = Ak*dGrad;
    solvedAk *= Matrix<double>(dGradient,Matrix::MATRIX_TRANSPOSE);
    solvedAk *= Matrix<double>(Ak,Matrix::MATRIX_TRANSPOSE);

    Matrix<double> divider = Matrix<double>(dGradient,Matrix::MATRIX_TRANSPOSE);
    divider *= Matrix<double>(Ak,Matrix::MATRIX_TRANSPOSE);
    divider *= dGradient;

    solvedAk /= divider;
    solvedAk = Ak - solvedAk;

    divider = dX* Matrix<double>(dX,Matrix::MATRIX_TRANSPOSE);
    divider /= (Matrix<double>(dX,Matrix::MATRIX_TRANSPOSE) * dGradient);

    solvedAk += divider;

    return solvedAk;
}

double ExtractorQuasiNewton::newtonRaphsonMethod(const Vector<double>& Xk,
                                                 const Vector<double>& S) {
    double cappa = 1.0;
    double tmpCappa = 1.0;



}

}
}
