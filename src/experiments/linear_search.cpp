#include "defines.h"

using namespace tsunami;

// Одномерная минимизация

void linsearch(const VectorDouble& oldX, double oldFunc, const VectorDouble& gradient,
               VectorDouble& p, VectorDouble& newX, double& newFunc,
               double stepMax, bool& check) {
    const double ALF = 1e-4;
    int N = oldX.rows();
    double summa = .0;
    for(int i=0; i < N; ++i){
        summa += p[i]*p[i];
    }

    summa = sqrt(summa);

    if(summa > stepMax) { // Масштабируем
        for(int i=0; i < N; ++i){
            p[i] *= stepMax / summa;
        }
    }

    double slope; // наклон
    for(int i=0; i < N; ++i){
        slope += g[i]*p[i];
    }

    if(slope >= .0){
        Q_ASSERT(false);
    }

    double test = .0, temp = .0;
    for(int i=0; i < N; ++i){
        temp = fabs(p[i])/std::max<double>(fabs(oldX[i]),1.0);
        if(temp > test) {
            test = temp;
        }
    }


    double toleranceLambda = TSUNAMI_DOUBLE_EPS / test;
    double lambda = 1.0;
    double tmpLambda = .0;

    while(true){
        // Переносим
        for(int i =0; i < N;++i){
            newX[i] = oldX[i] + lambda*p[i];
        }

        newFunc = FUNCTION(newX);

        double tmpFunc = .0, tmpLambda = .0;

        // Проверяем на допустимое значение
        if(lambda < toleranceLambda){
            newX = oldX;
            return;
        } else if(newFunc <= oldFunc + ALF*lambda*slope) {
            return;
        } else {
            if(lambda == 1.0) { // первый шаг
                tmpLambda = -slope(2.0*(newFunc-oldFunc-slope));
            }else{
                double rhs1,rhs2;

                rhs1 = newFunc - oldFunc - lambda*slope;
                rhs2 = tmpFunc - oldFunc - tmpLambda*slope;

                double a,b;

                a = (rhs1/(lambda*lambda)-rhs2/(tmpLambda*tmpLambda))/(lambda-tmpLambda);
                b = (-tmpLambda*rhs1/(lambda*lambda)+lambda*rhs2/(tmpLambda*tmpLambda))/(lambda-tmpLambda);

                if(a == .0){

                }
            }
        }




    }



}
