#include "ExtractorHookeJeeves.h"
#include "models/LibraryModel.h"
#include "models/ParameterModel.h"
#include <QDebug>

namespace tsunami{
namespace core{

ExtractorHookeJeeves::ExtractorHookeJeeves(DeviceType type, db::LibraryModel *library, const QList<int> &measures)
    : Extractor(type,library,measures) {

    int nParameters = library->countParameters();

    // TODO: remove temp code
    tolerances_.insert(TOLERANCE_STEP,1e-19);

    for(int i=0; i < nParameters; ++i){
        previousSteps_.insert( i, DBL_MAX );
        double diff = library->at(i).maximum() - library->at(i).minimum();
        currentSteps_.insert(i, diff);
        masks_.insert( i, HJ_HOLD );
    }

}

void ExtractorHookeJeeves::process() {
    currentFunctionError(functionError());
    qDebug() << currentFunctionError();

    if(stopped_) return;

    emit log( "Extraction started" );

    emit log( tr("Initial function error:%1").arg(currentFunctionError(),0,'g',20) );

    tempFunctionError_ = DBL_MAX;

    while(checkConvergence()){
        increaseIteration();
        emit log(tr("Iteration #%1").arg(iteration_));

        if( currentFunctionError() > tempFunctionError_ ){
            currentFunctionError( tempFunctionError_ );
            patternStep();
        }else{
            while(!findBestNearby()){
                if(!decreaseSteps()){
                    break;
                    // Error
                }
            }

        }


        emit log( QString("- Function error: %1").arg(currentFunctionError(),0,'g',20) );


    }
    emit log("Extraction finished");
    emit finished();
}

bool ExtractorHookeJeeves::findBestNearby() {
    double error;

    tempFunctionError_ = currentFunctionError();

    int nParameters = numberParameters();
    for(int i=0; i < nParameters; ++i){
        if(!fixed(i)){
            double value = fitted(i);
            double _step = step(i);
            if(testBoundary(i,value+_step)){
                fitted(i, value + _step );
                error = functionError();
                if( tempFunctionError_ > error ){
                    tempFunctionError_ = error;
                    mask(i,HJ_INC);
                    continue;
                }
            }else{
                qDebug() << "testBoundary failed: " << i;
            }

            if(testBoundary(i,value-_step)){
                fitted(i, value - _step);
                error = functionError();
                if(tempFunctionError_ > error){
                    tempFunctionError_ = error;
                    mask(i,HJ_DEC);
                    continue;
                }

            }else{
                qDebug() << "testBoundary failed: " << i;
            }

            fitted(i,value);
            mask(i,HJ_HOLD);

        }
    }


    bool found = ( currentFunctionError() > tempFunctionError_ );
    if(found){
        currentFunctionError( tempFunctionError_ );
    }
    return found;
}

void ExtractorHookeJeeves::patternStep() {
    int nParameters = numberParameters();
    for(int i=0; i < nParameters; ++i){
        if( !fixed(i) ){
            double value = fitted(i);
            double _step = step(i);
            switch(mask(i)){
            case HJ_INC:
                if( testBoundary(i,value + _step ) ){
                    fitted(i, value + _step );
                }
                break;
            case HJ_DEC:
                if( testBoundary(i,value-_step) ){
                    fitted( i, value-_step);
                }
                break;
            case HJ_HOLD:
            default:
                break;
            }
        }
    }

    tempFunctionError_ = functionError();
    return;
}

bool ExtractorHookeJeeves::decreaseSteps() {
    double factor = 0.5;
    int nParameters = numberParameters();

    saveSteps();

    for(int i=0; i < nParameters; ++i){
        if( !fixed(i) ){
            // TODO: Change algorithm decrease step, is fuck code =(
            step(i, step(i) * factor );
        }
    }


    return checkConvergence(false);

}

void ExtractorHookeJeeves::mask(int index, ExtractorHookeJeeves::Masks mask) {
    masks_.insert(index,mask);
}

const ExtractorHookeJeeves::Masks &ExtractorHookeJeeves::mask(int index) {
    return masks_[index];
}

bool ExtractorHookeJeeves::checkConvergence(bool showMessage){

    return Extractor::checkConvergence(showMessage);
}

}
}
