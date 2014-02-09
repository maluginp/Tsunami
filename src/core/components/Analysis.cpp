#include "Analysis.h"

#include "defines.h"
#include "components/Json.h"
namespace tsunami{

Analysis::Analysis()
{
}


void Analysis::parseJson(const QString &json) {
    QVariantMap analysis = QtJson::parse(json).toMap();

    type( analysis.value("type").toString() );

    QVariantList sources = analysis.value("sources").toList();
    numberSources_ = sources.count();

    for(int i=0; i < numberSources_; ++i){
        QVariantMap source = sources[i].toMap();
        sources_.append( source );
    }

}

QString Analysis::json() const {
    QVariantMap analysis;
    analysis.insert("type", typeJson() );

    analysis.insert("sources", sources_ );

    return QtJson::serializeStr( analysis );
}

AnalysisType Analysis::type() const {
    return type_;
}

void Analysis::type(const QString &type) {
    if(type == "dc"){
        type_ = ANALYSIS_DC;
    }else if(type == "ac"){
        type_ = ANALYSIS_AC;
    }else if(type == "tran"){
        type_ = ANALYSIS_TRAN;
    }
}

QString Analysis::typeJson() const {
    if(type_ == ANALYSIS_DC){
        return "dc";
    }else if(type_ == ANALYSIS_AC){
        return "ac";
    }else if(type_ == ANALYSIS_TRAN){
        return "tran";
    }
    return "";
}

void Analysis::appendSource(const QVariantMap &source) {
    numberSources_++;
    sources_.append( source );
}

void Analysis::sources(const QVariantList &sources) {
    sources_ = sources;
    numberSources_ = sources.count();
}

}
