#ifndef ANALYSISMODEL_H
#define ANALYSISMODEL_H
#include <QDateTime>
#include "defines.h"
namespace tsunami{
namespace db{

class AnalysisModel{
public:
    AnalysisModel();

    void id(int analysisId) { analysisId_ = analysisId; }
    void type( AnalysisType type ) { type_ = type; }
    void type( const QString& type );
    void deviceId( int deviceId ) { deviceId_ = deviceId; }
    void name( const QString& name) { name_ = name; }
    void addSource( const Source& source );
    const Source& findSource( const QString& node, SourceDirection direction );
    void removeSource(const Source& source);
    void sources( const QList<Source>& sources  ){ sources_ = sources; }
    void sourcesJson( const QString& json );
    void createAt( const QDateTime& createAt ){ createdAt_ = createAt; }
    void changeAt( const QDateTime& changeAt ){ changedAt_ = changeAt; }
    void enable(bool enable) { enable_ = enable; }

    const int& id() { return analysisId_; }
    const AnalysisType& type() { return type_; }
    QString typeJson();
    const QList<Source>& sources() { return sources_; }
    QList<Source> sources(SourceDirection direction);


    const QString& name() { return name_; }
    const QDateTime& createAt() { return createdAt_; }
    const QDateTime& changeAt() { return changedAt_; }
    const bool& enable() { return enable_; }
    const int& deviceId() { return deviceId_; }

    QString sourcesJson() const;
    bool sourceExists( const QString& node, SourceDirection direction  );


private:


    int analysisId_;
    int deviceId_;
    AnalysisType type_;
    QList<Source> sources_;
    QString name_;
    QDateTime changedAt_, createdAt_;
    bool enable_;

};



}
}


#endif // ANALYSISMODEL_H
