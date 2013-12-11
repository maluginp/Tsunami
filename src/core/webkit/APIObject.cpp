#include "APIObject.h"
//#include "dbstorage/analysisstorage.h"
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

    if(!sourcesJson.isEmpty()){
        emit openedAnalysis( sourcesJson );
    }

}

QString APIObject::test() {
    return "APIObject::test()";
}

void APIObject::saveAnalysis(const QVariantMap &sourcesJson){
    QList<tsunami::Source> sources;
    foreach(QString node, sourcesJson.keys()){
        tsunami::Source source;
//        source.mode()

    }
}

