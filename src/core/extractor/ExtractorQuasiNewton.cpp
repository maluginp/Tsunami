#include "ExtractorQuasiNewton.h"


namespace tsunami{
namespace core{

ExtractorQuasiNewton::ExtractorQuasiNewton(DeviceType type,
                                           db::LibraryModel *library,
                                           const QList<int> &measures)
    : Extractor(type,library,measures)
{
    int N = numberParameters();


}

void ExtractorQuasiNewton::process() {
    int N = numberParameters();

    Vector<double> gradient = solveGradient();
    Vector<double> lastGradient(N), dGradient(N);

    currentFunctionError_ = functionError();

    MatrixDouble Ak1(N,N,MatrixDouble::MATRIX_IDENTITY),
            Ak(N,N,MatrixDouble::MATRIX_ZERO);

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

        Ak1 = solveAk(Ak,dGradient,dX);



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

MatrixDouble ExtractorQuasiNewton::solveAk(const MatrixDouble &Ak,
                                             const Vector<double>& dGradient,
                                             const Vector<double>& dX) {
    MatrixDouble tmp(Ak,MatrixDouble::MATRIX_COPY);

    tmp *= dGradient;
    tmp *= MatrixDouble(dGradient,MatrixDouble::MATRIX_TRANSPOSE);
    tmp *= MatrixDouble(Ak,MatrixDouble::MATRIX_TRANSPOSE);

    MatrixDouble divider = MatrixDouble(dGradient,MatrixDouble::MATRIX_TRANSPOSE);
    divider *= MatrixDouble(Ak,MatrixDouble::MATRIX_TRANSPOSE);
    divider *= dGradient;

    tmp /= divider;

    MatrixDouble solvedAk(Ak);
    solvedAk -= tmp;

    divider = MatrixDouble(dX);
    divider *= MatrixDouble(dX,MatrixDouble::MATRIX_TRANSPOSE);
    divider /= (MatrixDouble(dX,MatrixDouble::MATRIX_TRANSPOSE) * dGradient);

    solvedAk += divider;

    return solvedAk;
}

double ExtractorQuasiNewton::newtonRaphsonMethod(const Vector<double>& Xk,
                                                 const Vector<double>& S) {
    double cappa = 1.0;
    double tmpCappa = 1.0;

//    double func = Xk



}

}
}
