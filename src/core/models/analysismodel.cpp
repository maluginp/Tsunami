#include "analysismodel.h"
#include "../components/json.h"
#include <QDateTime>

namespace tsunami{
namespace db{

AnalysisModel::AnalysisModel()
    : type_(ANALYSIS_UNKNOWN),
      changedAt_(QDateTime::currentDateTime()),
      createdAt_(QDateTime::currentDateTime()),
      enable_(true),
      deviceId_(-1),
      analysisId_(-1) {

}

void AnalysisModel::addSource( const Source& source ){
    sources_.append( source );
}

void AnalysisModel::type(const QString &type) {
    if(type.compare("dc") == 0){
        type_ = ANALYSIS_DC;
    }else if(type.compare("ac") == 0){
        type_ = ANALYSIS_AC;
    }else if(type.compare("tran") == 0){
        type_ = ANALYSIS_TRAN;
    }else{
        Q_ASSERT(false);
    }
}

void AnalysisModel::sourcesJson(const QString &json){
    sources_.clear();
    QVariantList sources = QtJson::parse( json ).toList();

    for(int i=0; i < sources.size(); ++i){
        QVariantMap sourceJson = sources[i];
        Source source;
        source.node = sourceJson.value( "node" ).toString();
        QString method = sourceJson.value("method").toString();

        // Set method
        if(method.compare("linear")==0){
            source.method = SOURCE_METHOD_LINEAR;
        }else if(method.compare("list") == 0){
            source.method = SOURCE_METHOD_LIST;
        }else if(method.compare("const") == 0){
            source.method = SOURCE_METHOD_CONST;
        }else{
            Q_ASSERT(false);
        }

        source.configuration = sourceJson.value("configuration",QVariantMap()).toMap();

        QString mode = sourceJson.value("mode").toString();
        if(mode.compare("voltage") == 0){
            source.mode = SOURCE_MODE_VOLTAGE;
        }else if(mode.compare("current") == 0){
            source.mode = SOURCE_MODE_CURRENT;
        }else if(mode.compare("gnd") == 0){
            source.mode = SOURCE_MODE_GND;
        }else{
            Q_ASSERT(false);
        }

        QString direction = sourceJson.value("direction").toString();
        if(direction.compare("input") == 0){
            source.direction = SOURCE_DIRECTION_INPUT;
        }else if(direction.compare("output") == 0){
            source.direction = SOURCE_DIRECTION_OUTPUT;
        }else{
            Q_ASSERT(false);
        }

        sources_.append(source);

    }


}

QString AnalysisModel::typeJson() {
    switch( type_ ){
    case ANALYSIS_DC: return QString("dc");
    case ANALYSIS_AC: return QString("ac");
    case ANALYSIS_TRAN: return QString("tran");
    default:
        break;
    }
    Q_ASSERT(false);
    return QString("unknown");
}

QString AnalysisModel::sourcesJson() {
    QString json;

    QVariantList sources;

    foreach(Source source, sources_){
        QVariantMap sourceJson;
        sourceJson.insert( "node", source.node );

        switch(source.method){
        case SOURCE_METHOD_CONST:
            sourceJson.insert("method","const");  break;
        case SOURCE_METHOD_LINEAR:
            sourceJson.insert("method","linear"); break;
        case SOURCE_METHOD_LIST:
            sourceJson.insert("method", "list"); break;
        default:
            Q_ASSERT(false);
        }

        sourceJson.insert( "configuration", source.configuration );

        switch(source.mode){
        case SOURCE_MODE_CURRENT: sourceJson.insert("mode", "current"); break;
        case SOURCE_MODE_VOLTAGE: sourceJson.insert("mode", "voltage"); break;
        case SOURCE_MODE_GND:     sourceJson.insert("mode", "gnd"); break;
        default:
            Q_ASSERT(false);
        }

        if(source.direction = SOURCE_DIRECTION_INPUT){
            sourceJson.insert("direction","input");
        }else if(source.direction == SOURCE_DIRECTION_OUTPUT){
            sourceJson.insert("direction","output");
        }else{
            Q_ASSERT(false);
        }


        sources.append( sourceJson );

    }

    json = QtJson::serializeStr( sources );

    return json;
}


}
}

