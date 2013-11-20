#include "hookejeevesmethod.h"
#include "extractor/extractor.h"
namespace tsunami{
namespace core{

HookeJeevesMethod::HookeJeevesMethod(Extractor *extractor)
    : OptimizeBase(extractor) {
}

void HookeJeevesMethod::run() {

    // TODO: Create correct method for convergnce
    lastFunctionError( functionError() );
//    saveFunctionError( baseFunctionError() );

    tempFunctionError( DBL_MAX );
    while(checkConvergence()){

        if( lastFunctionError() > tempFunctionError() ){
            lastFunctionError( tempFunctionError() );
            patternStep();
        }else{
            while(!findBestNearby()){
                if(!decreaseSteps()){
                    // Error
                }
            }
        }

//        saveFunctionError( lastFunctionError() );
//        saveFunctionError();


    }

}

// TODO: странная вещь но всегда все параметры enable
// необходимо доработка
void HookeJeevesMethod::findBestNearby() {
    double error;

    tempFunctionError( lastFunctionError() );

    int nParameters = countParameters();
    for(int i=0; i < nParameters; ++i){

        double value = extractor()->value( i );
        if(!extractor()->fixed(i)){

            extractor()->value(i, value + step(i) );
            error = functionError();

            if( tempFunctionError() > error ){
                tempFunctionError(error);
                mask(i,HJ_INC);
            }else{
                extractor()->value(i, value - step(i));
                error = functionError();
                if(tempFunctionError() > error){
                    tempFunctionError(error);
                    mask(i,HJ_DEC);
                }else{
                    extractor()->value(i,value);
                    mask(i,HJ_INC);
                }
            }
        }
    }

    return ( lastFunctionError() > tempFunctionError() );

}

void HookeJeevesMethod::patternStep() {

    int nParameters = countParameters();
    for(int i=0; i < nParameters; ++i){
        if( !extractor()->fixed(i) ){
            double value = extractor()->value(i);
            switch(mask(i)){
            case HJ_INC:
                extractor()->value(  i,  value + step(i) ); break;
            case HJ_DEC:
                extractor()->value( i, value-step(i)); break;
            case HJ_HOLD:
            default:
                break;
            }
        }
    }

    tempFunctionError( functionError() );
    return;
}

void HookeJeevesMethod::decreaseSteps() {
    double factor = 0.5;
    int nParameters = countParameters();

    saveSteps();

    for(int i=0; i < nParameters; ++i){
        if( !extractor()->fixed(i) ){
            // TODO: Change algorithm decrease step, is fuck code =(
            step(i, step() * factor );
        }
    }

    return;

}

void HookeJeevesMethod::mask(int index, HookeJeevesMethod::Masks mask) {
    Q_ASSERT( masks_.contains(index) );

    masks_[index] = mask;


}

HookeJeevesMethod::Masks HookeJeevesMethod::mask(int index) {
    Q_ASSERT( masks_.contains(index) );

    return masks_[index];
}


}
}
