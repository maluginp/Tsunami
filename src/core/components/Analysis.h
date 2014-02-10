#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "defines.h"

namespace tsunami{

class TSUNAMI_EXPORT Analysis {
public:
    Analysis();

    void parseJson(const QString& json);
    QString json() const;
    const int& numberSources() const;
    AnalysisType type() const;
    void type(const QString& type);
    QString typeJson() const;

    void appendSource(const QVariantMap& source);

    const QVariantMap &source(int i) const;

    void setSource(int i,const QVariantMap& source);
    void sources(const QVariantList &sources);
    const QVariantList& sources() const  { return sources_; }
private:
    AnalysisType type_;
    int numberSources_;

    QVariantList sources_;

};

}

#endif // ANALYSIS_H
