#include "QuasiNewtonExpr.h"

QuasiNewtonExpr::QuasiNewtonExpr()
{
}

VectorDouble QuasiNewtonExpr::optimize(const VectorDouble &vectorX, FUNCTION func, FUNC_GRADIENT grad) {

    int N = vectorX.rows();

    VectorDouble gradient = grad(vectorX);
    VectorDouble lastGradient(N), dGradient(N);

    double currentFunctionError_ = func(vectorX);

    qDebug() << "Start function value:" << currentFunctionError_;

    qDebug() << "Gradient";
    for(int i =0; i < N; ++i){
        qDebug() << "Grad["<<i<<"]"<<gradient[i];
    }


    MatrixDouble Ak1(N,N,MatrixDouble::MATRIX_IDENTITY),
            Ak(N,N,MatrixDouble::MATRIX_ZERO);

    VectorDouble S(N);

    VectorDouble Xk(N), Xk1(N), dX(N);

    Xk = vectorX;

//    for(int i=0; i < N; ++i){
//        Xk[i] = fitted(i);
//    }

    Xk1 = Xk;

    double cappa = .0;
    int iter = 0;
    while(true){
//        increaseIteration();
        iter++;
        Xk = Xk1;
        qDebug() << "Print Ak1";
        print(Ak1);

        MatrixDouble tmp(Ak1);

        S = -tmp*gradient;
        qDebug() << "S";
        print(S);

        double lambda;
        Xk1 = linSearch(func, Xk, gradient, S, lambda );

        qDebug() << "Lambda:" << lambda;
        qDebug() << "End linear search";
        qDebug() << "Old Xk";
        print(Xk);
        qDebug() << "New Xk";
        print(Xk1);
        lastGradient = gradient;
        gradient = grad(Xk1);

        dGradient = gradient - lastGradient;
        dX = Xk1 - Xk;

        Ak = Ak1;

        Ak1 = solveAk(Ak,dGradient,dX);


        double dXsum = .0;
        for(int i =0; i < N; ++i){
            dXsum += dX[i];
        }

        if(dXsum < TSUNAMI_DOUBLE_EPS){
            break;
        }



    }
    qDebug() << "Count iteration"<< iter;
    print(Xk1);
    return Xk1;
}

VectorDouble QuasiNewtonExpr::linSearch(FUNCTION func, const VectorDouble &oldX,
                                const VectorDouble& gradient,
                                VectorDouble &S, double& lambda) {
    const double ALF = 1e-4;
    const double stepMax = 100.0;

    // Число параметров
    int N = oldX.rows();
    VectorDouble x(N);
    VectorDouble newX = oldX;

    double summa = .0;

    for(int i=0; i < N; ++i){
        summa += S[i]*S[i];
    }

    summa = sqrt(summa);

    // Масштабируем
    if(summa > stepMax){
        for(int i=0; i < N; i++){
            S[i] *= stepMax/summa;
        }
    }

    qDebug() << "Scaled S";
    print(S);

    double slope = .0;
    for(int i=0; i < N; ++i){
        slope += gradient[i]*S[i];
    }

    qDebug() << "Slope:" << slope;

    Q_ASSERT(slope < .0);

    double test=.0, temp;
    for(int i=0; i < N; ++i){
        temp = abs(S[i])/std::max<double>(fabs(oldX[i]),1.0);
        if(temp > test){
            test = temp;
        }
    }

    double lambdaMin = TSUNAMI_DOUBLE_EPS / test;
    double lambda2 = .0;
    double f, f2 = .0,fold = func(oldX);
    lambda = 1.0;

    while(true){

        for(int i=0; i < N; ++i){
            x[i] = oldX[i]+lambda*S[i];
        }

        qDebug() << "New X";
        print(x);

//        x = oldX+lambda*S;
        f = func(x);

        qDebug() << QString("Func(newX)=%1\nFunc(oldX)=%2\nFold > Fnew : %3")
                    .arg(f).arg(fold).arg( ((fold > f) ? "true" : "false") );

        double tmpLambda;

        if(f <= fold+ALF*lambda*slope){
            break;
        }else{
            if(lambda == 1.0){
                tmpLambda = -slope/(2.0*(f - fold -slope));
            }else{
                double rhs1 = f - fold - lambda*slope;
                double rhs2 = f2 - fold - lambda2*slope;
                double a = (rhs1/(lambda*lambda)-rhs2/(lambda2*lambda2))/(lambda-lambda2);
                double b= (-lambda2*rhs1/(lambda*lambda) +  lambda*rhs2/(lambda2*lambda2))/(lambda-lambda2);

                double disc;
                if(a == .0){
                    tmpLambda = -slope/(2.0*b);
                }else{
                    disc = b*b-3.0*a*slope;
                    if(disc < .0){
                        tmpLambda = 0.5*lambda;
                    }else if(b <= .0){
                        tmpLambda = (-b +sqrt(disc))/(3.0*a);
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
        f2 = f;
        lambda = std::max<double>(tmpLambda,0.1*lambda);

//        newX = x;

        qDebug() << QString("Lambda(i-i): %1,\nLambda (i): %2")
                    .arg(lambda2)
                    .arg(lambda);
    }


    return x;

}

void QuasiNewtonExpr::print(const MatrixDouble &matrix) {
    qDebug() << QString("Matrix %1x%2").arg(matrix.rows()).arg(matrix.columns());
    for(int i=0; i < matrix.rows(); ++i){
        QStringList line;
        for(int j=0; j < matrix.columns(); ++j){
//            qDebug() << QString("m(%1,%2)=%3").arg(i).arg(j).arg(matrix(i,j));
              line.append( QString::number(matrix(i,j)) );
        }

        qDebug() << line.join("\t");
    }

    qDebug() << "\n";

}

void QuasiNewtonExpr::print(const VectorDouble &vector) {
    for(int i=0; i < vector.rows(); ++i){
        qDebug() << QString("v[%1]=%2").arg(i).arg(vector[i]);
    }
    qDebug() << "\n";

}

MatrixDouble QuasiNewtonExpr::solveAk(const MatrixDouble &Ak, const VectorDouble &dGradient, const VectorDouble &dX) {
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

VectorDouble QuasiNewtonExpr::modifyS(const VectorDouble &S, double cappa) {
    int N = S.rows();

    VectorDouble modifiedS(N);

    for(int i=0; i < N; ++i){
        modifiedS[i] = S[i]*cappa;
    }

    return modifiedS;
}
