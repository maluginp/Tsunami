#include "ExtractorQuasiNewton.h"
#include "Log.h";

namespace tsunami{
namespace core{

ExtractorQuasiNewton::ExtractorQuasiNewton(DeviceType type,
                                           db::LibraryModel *library,
                                           const QList<int> &measures)
    : Extractor(type,library,measures)
{
}

void ExtractorQuasiNewton::process() {
    int N = numberParameters();
    const double TOLX = 4 * TSUNAMI_DOUBLE_EPS, STEPMAX = 100.0;

    MatrixDouble Ak1(N,N,MatrixDouble::MATRIX_IDENTITY);

    currentFunctionError_ = functionError();

    VectorDouble gradient = solveGradient(), dGradient(N);
    double summa=.0;

    VectorDouble Xk(N), xi(N);
    for(int i=0; i < N; ++i){
        xi[i] = -gradient[i];
        Xk[i] = fitted(i);

        summa += Xk[i]*Xk[i];
    }

    double stepMax = STEPMAX*std::max<double>(sqrt(summa),double(N));

    VectorDouble Xk1(N);
    while(true){
        increaseIteration();

        log::logTrace() << "Iteration #" << iteration_;
        emit log(tr("<font color='green'>Iteration #%1</font>").arg(iteration_));

        Xk1 = linearSearch(Xk,currentFunctionError_,gradient,xi,stepMax);

        for(int i=0; i < N; ++i){
            fitted(i,Xk1[i]);
        }

        double fp = functionError();

        if(fp < currentFunctionError_){
            currentFunctionError( fp );
            for(int i=0; i < N; ++i){
                xi[i] = Xk1[i]-Xk[i];
                Xk[i] = Xk1[i];
            }



        }else{
            emit log( QString("- Function error: %1").arg(currentFunctionError(),0,'g',20) );
            break;
        }

        emit log( QString("- Function error: %1").arg(currentFunctionError(),0,'g',20) );

        double test = .0;
        double temp = .0;
        for(int i=0; i < N; ++i){
            temp = fabs(xi[i])/std::max<double>(fabs(Xk[i]),1.0);
            if(temp > test){
                test = temp;
            }
        }

        if(test < TOLX){
            break;
        }

        // Save gradient
        for(int i=0; i < N; ++i){
            dGradient[i] = gradient[i];
        }

        gradient = solveGradient();
        test = .0;
        double den = std::max<double>(fp,1.0);

        for(int i=0; i < N; ++i){
            temp = fabs(gradient[i])*std::max<double>(fabs(Xk[i]),1.0)/den;
            if(temp > test){
                test = temp;
            }
        }

        double tolg = tolerances_[TOLERANCE_FUNCTION];
//        double tolg = tolerances_[TOLERANCE_GRADIENT];

        if(test < tolg){
            break;
        }

        dGradient = gradient - dGradient;
        VectorDouble hdg(N);

        for(int i=0;i < N; ++i){
            for(int j=0; j < N; ++j){
                hdg[i] += Ak1.at(i,j)*dGradient[j];
            }
        }

        double fac=.0, fae=.0,sumdg=.0,sumxi=0.0,fad = .0;

        for(int i=0; i < N; ++i){
            fac += dGradient[i]*xi[i];
            fae += dGradient[i]*hdg[i];
            sumdg += sqrt(dGradient[i]);
            sumxi += sqrt(xi[i]);
        }

        if(fac > sqrt(TSUNAMI_DOUBLE_EPS*sumdg*sumxi)){
            fac = 1.0/fac;
            fad = 1.0/fae;

            for(int i =0; i < N; ++i){
                dGradient[i] = fac*xi[i]-fad*hdg[i];
            }
            for(int i=0; i < N; ++i){
                for(int j=0; j < N; ++j){
                    Ak1.at(i,j) += fac*xi[i]*xi[j]
                            -fad*hdg[i]*hdg[j]
                            + fae*dGradient[i]*dGradient[j];
                    Ak1.at(j,i) = Ak1.at(i,j);
                }
            }

        }

        for(int i=0; i < N; ++i){
            xi[i] = .0;
            for(int j=0; j <N; ++j){
                xi[i] -= Ak1.at(i,j)*gradient[j];
            }
        }
    }


    emit log("Extraction finished");
    emit finished();
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

    qDebug() << "dGradient";
    print(dGradient);
    qDebug() << "dX";
    print(dX);

    MatrixDouble tmp(Ak,MatrixDouble::MATRIX_COPY);
    qDebug() << "Ak";
    print(tmp);
    tmp *= dGradient;
    qDebug() << "After mul gradient";
    print(tmp);

    MatrixDouble tranDGrad = MatrixDouble(dGradient,MatrixDouble::MATRIX_TRANSPOSE);
    qDebug() << "Transpose dGradient";
    print(tranDGrad);

    tmp *= tranDGrad;
    qDebug() << "After dGradient(T)";
    print(tmp);
    MatrixDouble tranAk(Ak,MatrixDouble::MATRIX_TRANSPOSE);
    qDebug() << "Tran Ak";
    print(tranAk);

    tmp *= tranAk;
    qDebug() << "Afet mul tranAk";
    print(tmp);

    MatrixDouble divider = MatrixDouble(dGradient,MatrixDouble::MATRIX_TRANSPOSE);
    divider *= MatrixDouble(Ak,MatrixDouble::MATRIX_TRANSPOSE);
    divider *= dGradient;
    print(divider);
    tmp /= divider;

    MatrixDouble solvedAk(Ak);
     print(solvedAk);
     print(tmp);
    solvedAk -= tmp;

    divider = MatrixDouble(dX);
    divider *= MatrixDouble(dX,MatrixDouble::MATRIX_TRANSPOSE);
    divider /= (MatrixDouble(dX,MatrixDouble::MATRIX_TRANSPOSE) * dGradient);

    print(solvedAk);
    print(divider);

    solvedAk += divider;

    return solvedAk;
}

VectorDouble ExtractorQuasiNewton::linearSearch(const VectorDouble &oldX,
                                                const double fold,
                                                const VectorDouble &gradient,
                                                VectorDouble &S,double stepMax) {

    const double ALF = 1e-4;
//    const double stepMax = 100.0;

    // Число параметров
    int N = oldX.rows();
    VectorDouble x(N), newX = oldX;

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
    double f, f2 = .0;
    double lambda = 1.0;
    double tmpLambda;
    f = fold;

    qDebug() << "Fold=" << fold;

    while(true){
        bool failBoundary = false;
        for(int i=0; i < N; ++i){
            x[i] = oldX[i]+lambda*S[i];

            if(!fixed(i)){
                if(!testBoundary(i, x[i])){
                    qDebug() << QString("%1 not in boundary(%2 <= %3 <= %4")
                                .arg(i).arg(minimum(i)).arg(x[i]).arg(maximum(i));
                    qDebug() << QString("Old X[%1]=%2").arg(i).arg(oldX[i]);
                    qDebug() << QString("S[%1]=%2").arg(i).arg(S[i]);

                    failBoundary = true;
                }

            }
            //            fitted(i, x[i]);
        }

        if(!failBoundary){
            for(int i=0; i < N; ++i){
                fitted(i, x[i]);
            }
            f = functionError();
            qDebug() << QString("Func(newX)=%1\nFunc(oldX)=%2\nFold > Fnew : %3")
                        .arg(f).arg(fold).arg( ((fold > f) ? "true" : "false") );
        }



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

double ExtractorQuasiNewton::newtonRaphsonMethod(const Vector<double>& Xk,
                                                 const Vector<double>& S) {
    double cappa = 1.0;
    double tmpCappa = 1.0;

//    double func = Xk



}

void ExtractorQuasiNewton::print(const MatrixDouble &matrix) {
    qDebug() << QString("Matrix %1x%2").arg(matrix.rows()).arg(matrix.columns());
    for(int i=0; i < matrix.rows(); ++i){
        QStringList line;
        for(int j=0; j < matrix.columns(); ++j){
//            qDebug() << QString("m(%1,%2)=%3").arg(i).arg(j).arg(matrix(i,j));
            line.append( QString::number(matrix(i,j),'f',16) );
        }

        qDebug() << line.join("\t");
    }

    qDebug() << "\n";

}

void ExtractorQuasiNewton::print(const VectorDouble &vector) {
    for(int i=0; i < vector.rows(); ++i){
        qDebug() << QString("v[%1]=%2").arg(i).arg(QString::number(vector[i],'f',16));
    }
    qDebug() << "\n";
}

}
}
