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

QList<double> AnalysisModel::analysisValues(int i) {
    Q_ASSERT( i < analyses_.count());

    QVariantMap analysis = analyses_[i].toMap();

    QList<double> values;
    if(type_ == ANALYSIS_AC){
        double start = analysis.value("start").toDouble();
        double stop  = analysis.value("stop").toDouble();
        double points = analysis.value("points").toInt();
        double step = fabs((stop-start)/points);

        for(int i=0; i <= points; ++i){
            values.append( start+step*i );
        }

    }else{

        double start = analysis.value("start").toDouble();
        double step  = analysis.value("step").toDouble();
        double stop  = analysis.value("stop").toDouble();

        int numberValues = static_cast<int>(fabs((stop-start)/step));
        for(int i=0; i<= numberValues;++i){
            values.append(start+step*i);
        }
    }

    return values;
}

void AnalysisModel::analysesJson(const QString &json) {
    analyses_ = QtJson::parse(json).toList();
}

QString AnalysisModel::analysesJson() {
    return QtJson::serializeStr(analyses_);
}

QString AnalysisModel::debug() const {
    QString debug = QString("AnalysisModel\n"
                          "ID:%1, TYPE:%2, DEVICE_ID:%3, NAME:%4,"
                          "ENABLE:%5, CREATE_AT:%6, CHANGE_AT:%7\n"
                          "SOURCES INPUTS:%8\nSOURCE OUTPUTS:%9")
                  .arg(analysisId_).arg(typeJson()).arg(deviceId_)
                  .arg(name_).arg(enable_)
                  .arg(createdAt_.toString())
                  .arg(changedAt_.toString())
                  .arg(sourcesJson(SOURCE_DIRECTION_INPUT))
                  .arg(sourcesJson(SOURCE_DIRECTION_OUTPUT));
    return debug;
}


}
}

