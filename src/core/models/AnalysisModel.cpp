#include "AnalysisModel.h"
#include "components/Json.h"


namespace tsunami{
namespace db{

AnalysisModel::AnalysisModel()
    : type_(ANALYSIS_UNKNOWN),
      changedAt_(QDateTime::currentDateTime()),
      createdAt_(QDateTime::currentDateTime()),
      enable_(true),
      deviceId_(-1),
      analysisId_(-1)
{

}

void AnalysisModel::addSource(Source *source ){
    sources_.append( source );
}

//const Source &AnalysisModel::findSource(const QString &node, SourceDirection direction) {
//    int nSources = sources_.size();
//    for(int i=0; i < nSources; i++ ){
//        if(sources_[i].direction() == direction && sources_[i].node().compare(node) == 0){
//            return sources_[i];
//        }
//    }
//    Q_ASSERT(false);
//}

//void AnalysisModel::removeSource(const Source &source) {
//    QList<Source>::iterator it = sources_.begin();
//    for(; it != sources_.end(); ++it){
//        if(it->direction() == source.direction() && it->node().compare(source.node()) == 0){
//            sources_.erase( it );
//        }
//    }
//}

void AnalysisModel::type(const QString &type) {
    if(type.compare("dc") == 0){
        type_ = ANALYSIS_DC;
    }else if(type.compare("ac") == 0){
        type_ = ANALYSIS_AC;
    }else if(type.compare("tran") == 0){
        type_ = ANALYSIS_TRAN;
    }else{
//        Q_ASSERT(false);
    }
}

void AnalysisModel::sourcesJson(const QString &json){
//    sources_.clear();
    QVariantList sources = QtJson::parse( json ).toList();

    for(int i=0; i < sources.count(); i++) {
        QVariantMap sourceJson = sources[i].toMap();
        Source* source = new Source();
        source->node(sourceJson.value( "node" ).toString());
        source->type(sourceJson.value("type").toString());
        source->method(sourceJson.value("method").toString());
        source->configurations(sourceJson.value("configuration",QVariantMap()).toMap()); source->mode( sourceJson.value("mode").toString() );
        source->direction(sourceJson.value("direction").toString());

        sources_.append(source);

    }


}


QString AnalysisModel::typeJson() const {
    switch( type_ ){
    case ANALYSIS_DC: return QString("dc");
    case ANALYSIS_AC: return QString("ac");
    case ANALYSIS_TRAN: return QString("tran");
    default:
        break;
    }
//    Q_ASSERT(false);
    return QString("unknown");
}

const QList<Source *>& AnalysisModel::sources() const {
    return sources_;
}

QList<Source *> AnalysisModel::sources(SourceDirection direction) const {
    QList<Source*> sources;

    foreach(Source* source,sources_){
        if(source->direction() == direction){
            sources.append(source);
        }
    }
    return sources;
}


void AnalysisModel::clearSources(SourceDirection direction) {
    foreach(Source* source,sources_){
        if(source->direction() == direction){
            sources_.removeOne(source);
            delete source;
        }
    }
//    sources_.clear();
}

//QList<Source> AnalysisModel::sources(SourceDirection direction) const{
//    QList<Source> sources;
//    int nSources = sources_.size();
//    for(int i=0; i < nSources; ++i){
//        if(sources_[i].direction() == direction){
//            sources.append( sources_[i] );
//        }
//    }

//    return sources;

//}

QString AnalysisModel::sourcesJson(SourceDirection direction) const {
    QString json;

    QVariantList sources;

    foreach(Source* source, sources_){
        if(source->direction() == direction){
            sources.append( source->json() );
        }
    }

    json = QtJson::serializeStr( sources );

    return json;
}

void AnalysisModel::analyses(const QVariantList &analyses) {
    analyses_ = analyses;
}

const QVariantList &AnalysisModel::analyses() const {
    return analyses_;
}

void AnalysisModel::addAnalysis(const QVariantMap &analysis) {
    analyses_.append(analysis);
}

int AnalysisModel::numberAnalyses() {
    return analyses_.count();
}

QVariantMap AnalysisModel::analysis(int i) const {
    return analyses_[i].toMap();
}

void AnalysisModel::analysesJson(const QString &json) {
    analyses_ = QtJson::parse(json).toList();
}

QString AnalysisModel::analysesJson() {
    return QtJson::serializeStr(analyses_);
}

//bool AnalysisModel::sourceExists(const QString &node, SourceDirection direction) {
//    int nSources = sources_.size();
//    for(int i=0; i < nSources; i++ ){
//        if(sources_[i].direction() == direction && sources_[i].node().compare(node) == 0){
//            return true;
//        }
//    }

//    return false;
//}

//Source AnalysisModel::source(int i) const {
//    return sources_[i];
//}


}
}

