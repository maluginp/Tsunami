#include "APIObject.h"
//#include "dbstorage/analysisstorage.h"

APIObject::APIObject(QObject *parent) :
    QObject(parent), analysis_(0) {

    setObjectName("Api");


}

void APIObject::openAnalysis( const tsunami::db::AnalysisModel* analysis ) {
    QList<tsunami::Source> sources = analysis->sources();

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

    if(!sourcesJson.isEmpty()){
        emit openedAnalysis( sourcesJson );
    }

}

QString APIObject::test() {
    return "APIObject::test()";
}

