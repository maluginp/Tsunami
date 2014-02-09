#include "APIObject.h"
#include "dbstorage/DbStorages.h"
#include "Log.h"
#include "defines.h"

using namespace tsunami;

APIObject::APIObject(int deviceId,QObject *parent) :
    QObject(parent), analysis_(0),deviceId_(deviceId) {

    setObjectName("Api");


}

void APIObject::openAnalysis( db::AnalysisModel* analysis ) {
    log::logTrace() << "Opening analysis";
    if(!analysis){
        log::logError() << "Analysis object is empty";
        return;
    }

//    analysis_ = analysis;

//    QList<Source> sources = analysis_->sources();
//    QVariantMap sourcesJson;

//    foreach( Source source, sources ){
//        if( source.direction() == SOURCE_DIRECTION_INPUT ){
//            QVariantMap sourceJson;
//            sourceJson.insert( "mode", source.modeJson() );
//            sourceJson.insert("method", source.methodJson());
//            sourceJson.insert("config", source.configurations());
//            sourceJson.insert("name",source.name());
//            sourcesJson.insert( source.node(), sourceJson );
//        }
//    }

//    if(!sourcesJson.isEmpty()){
//    emit openedAnalysis(analysis_->id(), sourcesJson );
//    }

}

QString APIObject::test() {
    return "APIObject::test()";
}

int APIObject::saveAnalysis(const QVariantMap &sourcesJson){
    log::logTrace() << "Saving analysis";
    db::DeviceStorage* storage = db::DeviceStorage::instance();
    db::DeviceModel* device = storage->openDevice( deviceId_ );

    QList<Source> sources;
    foreach(QString node, sourcesJson.keys()){
        QVariantMap sourceJson = sourcesJson[node].toMap();
        Source source;
        source.mode( sourceJson.value("mode").toString() );
//        sourceJson.remove( "mode" );
        source.method( sourceJson.value("method").toString() );
//        source.name( sourceJson.value("name").toString() );
        source.node( node );
        source.configurations( sourceJson.value("config",QVariantMap()).toMap() );
        sources << source;
    }

    if(device->type() == DEVICE_NMOS || device->type() == DEVICE_PMOS){
        sources.append(Source("D",SOURCE_MODE_CURRENT,
                                  SOURCE_DIRECTION_OUTPUT,
                                  SOURCE_METHOD_UNKNOWN));
    }else if(device->type() == DEVICE_PFET || device->type() == DEVICE_NFET){
        sources.append(Source("D",SOURCE_MODE_CURRENT,
                                  SOURCE_DIRECTION_OUTPUT,
                                  SOURCE_METHOD_UNKNOWN));
        sources.append(Source("G",SOURCE_MODE_CURRENT,
                                  SOURCE_DIRECTION_OUTPUT,
                                  SOURCE_METHOD_UNKNOWN));
    }else {
        int nSources = sources.count();
        for(int i=0; i < nSources; ++i){
            if(sources[i].mode() != SOURCE_MODE_GND){
                SourceMode mode;
                if( sources[i].mode() == SOURCE_MODE_VOLTAGE ){
                    mode = SOURCE_MODE_CURRENT;
                }else if(sources[i].mode() == SOURCE_MODE_CURRENT){
                    mode = SOURCE_MODE_VOLTAGE;
                }
                sources.append( Source(sources[i].node(), mode,
                                       SOURCE_DIRECTION_OUTPUT,
                                       SOURCE_METHOD_UNKNOWN));
            }
        }
    }

    emit savedAnalysis(sources);
    return -1;
}

