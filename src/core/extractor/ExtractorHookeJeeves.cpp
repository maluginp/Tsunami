#include "ExtractorHookeJeeves.h"

#include <Log.h>
namespace tsunami{
namespace core{

ExtractorHookeJeeves::ExtractorHookeJeeves(DeviceType type, db::LibraryModel *library, const QList<int> &measures)
    : Extractor(type,library,measures) {

    int nParameters = library->countParameters();


    for(int i=0; i < nParameters; ++i){
        masks_.insert( i, HJ_HOLD );
    }



}

void ExtractorHookeJeeves::process() {
    log::logTrace() << "Start Hooke-Jeeves Minimisation";

    currentFunctionError(functionError());
    log::logTrace() << QString("Initial error: %1")
                       .arg(currentFunctionError());

    if(stopped_) return;

    emit log( "<b>Extraction started</b>" );

    emit log( tr("Initial function error:%1").arg(currentFunctionError(),0,'g',20) );

    tempFunctionError_ = DBL_MAX;

    while(checkConvergence()){
        increaseIteration();
        log::logTrace() << "Iteration #" << iteration_;
        emit log(tr("<font color='green'>Iteration #%1</font>").arg(iteration_));

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
    log::logTrace() << "Find best nearby";
    tempFunctionError_ = currentFunctionError();
//    emit log(tr("Find best nearby"));
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
            }

            if(testBoundary(i,value-_step)){
                fitted(i, value - _step);
                error = functionError();
                if(tempFunctionError_ > error){
                    tempFunctionError_ = error;
                    mask(i,HJ_DEC);
                    continue;
                }

            }

            fitted(i,value);
            mask(i,HJ_HOLD);

        }
    }


    bool found = ( currentFunctionError() > tempFunctionError_ );
    if(found){
        currentFunctionError( tempFunctionError_ );
        emit log("Found best nearby");
    }else{
//        emit log("Best nearby not found");
    }

    log::logTrace() << "Found best nearby: "
                    << ((found)?"true":"false");



    return found;
}

void ExtractorHookeJeeves::patternStep() {
    int nParameters = numberParameters();
    log::logTrace() << "Pattern step";
    emit log("Pattern step");
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
    log::logTrace() << "Pattern step result: " << tempFunctionError_;
    return;
}

bool ExtractorHookeJeeves::decreaseSteps() {
    double factor = 0.5;
    int nParameters = numberParameters();


//    emit log("Decrease step");
    saveSteps();

    for(int i=0; i < nParameters; ++i){
        if( !fixed(i) ){
            // TODO: Change algorithm decrease step, is fuck code =(
            step(i, step(i) * factor );
        }
    }

    QString dbg = QString("Decrease steps\n").append(debugSteps());
    log::logTrace() << dbg;
//    emit log(dbg);


    return checkConvergence(false);

}

void ExtractorHookeJeeves::mask(int index, ExtractorHookeJeeves::Masks mask) {
    masks_.insert(index,mask);
}

const ExtractorHookeJeeves::Masks &ExtractorHookeJeeves::mask(int index) {
    return masks_[index];
}

bool ExtractorHookeJeeves::checkConvergence(bool showMessage){
    bool convergenced = Extractor::checkConvergence(showMessage);
    log::logTrace() << "Check convergence: "
                    << ((convergenced) ? "true" : "false");
    return convergenced;
}

}
}
