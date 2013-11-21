#include "dataset.h"
#include "dbstorage/measurestorage.h"

namespace tsunami{
namespace core{

Dataset::Dataset() {

}

bool Dataset::load(AnalysisType type, DeviceType device, const QVariantMap &attr) {

    QVariantMap criteria;

    switch(type){
    case ANALYSIS_AC: criteria.insert("analysis","ac"); break;
    case ANALYSIS_DC: criteria.insert("analysis","dc"); break;
    case ANALYSIS_TRAN: criteria.insert("analysis","tran"); break;
    default:
        break;
//        Q_ASSERT(false);
    }

    switch(device){
    case DEVICE_NBJT:
        criteria.insert("device","nbjt"); break;
    case DEVICE_PBJT:
        criteria.insert("device","pbjt"); break;
    case DEVICE_NFET:
        criteria.insert("device","nfet"); break;
    case DEVICE_PFET:
        criteria.insert("device","pfet"); break;
    case DEVICE_NMOS:
        criteria.insert("device","nmos"); break;
    case DEVICE_PMOS:
        criteria.insert("device","pmos"); break;
    case DEVICE_DIODE:
        criteria.insert("device","diode"); break;
    case DEVICE_RESISTOR:
        criteria.insert("device","res");  break;
    case DEVICE_CAPACITOR:
        criteria.insert("device","cap"); break;
    }

    db::MeasureStorage* storage = db::MeasureStorage::instance();
    measures_ = storage->findMeasure( criteria );

    // Check attributes
    foreach( QString key, attr.keys() ){
        QVariant value = attr.value( key );
        QList<db::MeasureModel>::iterator it = measures_.begin();
        while(it != measures_.end()){
            if(!it->hasAttr( key, value )){
                measures_.erase( it );
            }else{
                ++it;
            }
        }
    }

    return true;

}

void Dataset::begin() {
    currentMeasure_ = measures_.begin();
}

bool Dataset::isNext() {
    return ( currentMeasure_ != measures_.end() );
}

const db::MeasureModel& Dataset::next(){
     db::MeasureModel measure = *currentMeasure_; //.pointer();
     currentMeasure_++;

     return measure;


}


}
}
