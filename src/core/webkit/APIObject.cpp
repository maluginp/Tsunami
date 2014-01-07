#include "APIObject.h"
#include "dbstorage/AnalysisStorage.h"
#include <QDebug>
#include "defines.h"
APIObject::APIObject(QObject *parent) :
    QObject(parent), analysis_(0) {

    setObjectName("Api");


}

void APIObject::openAnalysis( tsunami::db::AnalysisModel* analysis ) {
    analysis_ = analysis;
    QList<tsunami::Source> sources = analysis_->sources();

    QVariantMap sourcesJson;
    foreach( tsunami::Source source, sources ){
        if( source.direction() == tsunami::SOURCE_DIRECTION_INPUT ){
            QVariantMap sourceJson;
            sourceJson.insert( "mode", source.modeJson() );
            sourceJson.insert("method", source.methodJson());
            sourceJson.insert("config", source.configurations());
            sourceJson.insert("name",source.name());
            sourcesJson.insert( source.node(), sourceJson );
        }
    }

//    if(!sourcesJson.isEmpty()){
        emit openedAnalysis( sourcesJson );
//    }

}

QString APIObject::test() {
    return "APIObject::test()";
}

void APIObject::saveAnalysis(const QVariantMap &sourcesJson){
    QList<tsunami::Source> sources;
    foreach(QString node, sourcesJson.keys()){
        QVariantMap sourceJson = sourcesJson[node].toMap();
        tsunami::Source source;
        source.mode( sourceJson.value("mode").toString() );
//        sourceJson.remove( "mode" );
        source.method( sourceJson.value("method").toString() );
//        source.name( sourceJson.value("name").toString() );
        source.node( node );
        source.configurations( sourceJson.value("config",QVariantMap()).toMap() );

        sources << source;
    }

    int nSources = sources.count();
    for(int i=0; i < nSources; ++i){
        if(sources[i].mode() != tsunami::SOURCE_MODE_GND){
            tsunami::SourceMode mode;
            if( sources[i].mode() == tsunami::SOURCE_MODE_VOLTAGE ){
                mode = tsunami::SOURCE_MODE_CURRENT;
            }else if(sources[i].mode() == tsunami::SOURCE_MODE_CURRENT){
                mode = tsunami::SOURCE_MODE_VOLTAGE;
            }
            sources.append( tsunami::Source(sources[i].node(), mode,
                                            tsunami::SOURCE_DIRECTION_OUTPUT,
                                            tsunami::SOURCE_METHOD_CONST) );
        }
    }

    emit savedAnalysis(sources);

}

