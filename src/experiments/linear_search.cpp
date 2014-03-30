#include "defines.h"

//using namespace tsunami;
#include "linear_search.h"

// Одномерная минимизация
void linsearch(const VectorDouble& oldX, double oldFunc, const VectorDouble& gradient,
               VectorDouble& p, VectorDouble& newX, double& newFunc,
               double stepMax, bool& check, FUNCTION func) {
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
        slope += gradient[i]*p[i];
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
    double lambda2 = 0.0;
//    double tmpLambda = .0;
    double disc;

    while(true){
        // Переносим
        for(int i =0; i < N;++i){
            newX[i] = oldX[i] + lambda*p[i];
        }

        newFunc = func(newX);

        double tmpFunc = .0, tmpLambda = .0;

        // Проверяем на допустимое значение
        if(lambda < toleranceLambda){
            newX = oldX;
            return;
        } else if(newFunc <= oldFunc + ALF*lambda*slope) {
            return;
        } else {
            if(lambda == 1.0) { // первый шаг
                tmpLambda = -slope/(2.0*(newFunc-oldFunc-slope));
            }else{
                double rhs1,rhs2;

                rhs1 = newFunc - oldFunc - lambda*slope;
                rhs2 = tmpFunc - oldFunc - lambda2*slope;

                double a,b;

                a = (rhs1/(lambda*lambda)-rhs2/(lambda2*lambda2))/(lambda-lambda2);
                b = (-lambda2*rhs1/(lambda*lambda)+lambda*rhs2/(lambda2*lambda2))/(lambda-lambda2);

                if(a == .0){
                    tmpLambda = -slope*(2.0*b);
                }else{
                    disc = b*b-3.0*a*slope;
                    if(disc < .0){
                        tmpLambda = 0.5*lambda;
                    }else if(b <= .0){
                        tmpLambda = (-b+sqrt(disc))/(3.0*a);
                    }else{
                        tmpLambda = -slope/(b+sqrt(disc));
                    }
                }
                if(tmpLambda > 0.5*lambda){
                    tmpLambda = 0.5*lambda;
                }
            }
        }

        lambda2 = lambda;
        tmpFunc = newFunc;

        lambda = std::max<double>(tmpLambda,0.1*lambda);
    }



}
