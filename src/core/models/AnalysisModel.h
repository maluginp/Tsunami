#ifndef ANALYSISMODEL_H
#define ANALYSISMODEL_H
#include <QDateTime>
#include "defines.h"
#include "components/Analysis.h"

namespace tsunami{
namespace db{

class TSUNAMI_EXPORT AnalysisModel{
public:
    AnalysisModel();

    void id(int analysisId) { analysisId_ = analysisId; }
    void type( AnalysisType type ) { type_ = type; }
    void type( const QString& type );
    void deviceId( int deviceId ) { deviceId_ = deviceId; }
    void name( const QString& name) { name_ = name; }
    void addSource(Source* source);

//    const Source& findSource( const QString& node, SourceDirection direction );

//    void removeSource(const Source& source);
//    void sources( const QList<Source>& sources  ){ sources_ = sources; }
    void sourcesJson( const QString& json );
    void createAt( const QDateTime& createAt ){ createdAt_ = createAt; }
    void changeAt( const QDateTime& changeAt ){ changedAt_ = changeAt; }
    void enable(bool enable) { enable_ = enable; }
    void analysis( Analysis* analysis );
    void analysis(const QString& analysisJson);

    const int& id() { return analysisId_; }
    const AnalysisType& type() { return type_; }
    QString typeJson() const;
    const QList<Source*>& sources() const { return sources_; }
//    QList<Source> sources(SourceDirection direction) const;


    const QString& name() const { return name_; }
    const QDateTime& createAt() const { return createdAt_; }
    const QDateTime& changeAt() const { return changedAt_; }
    const bool& enable() const { return enable_; }
    const int& deviceId() const { return deviceId_; }

    const Analysis* analysis() const;

    void clearSources();

    QString sourcesJson() const;
//    bool sourceExists( const QString& node, SourceDirection direction  );
//    Source source(int i) const;

private:


    int analysisId_;
    int deviceId_;
    AnalysisType type_;

    Analysis* analysis_;
    QList<Source*> sources_;
    QString name_;
    QDateTime changedAt_, createdAt_;
    bool enable_;

};



}
}


#endif // ANALYSISMODEL_H
