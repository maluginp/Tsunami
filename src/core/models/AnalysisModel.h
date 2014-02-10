#ifndef ANALYSISMODEL_H
#define ANALYSISMODEL_H
#include <QDateTime>
#include "defines.h"


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

    void sourcesJson( const QString& json );
    void createAt( const QDateTime& createAt ){ createdAt_ = createAt; }
    void changeAt( const QDateTime& changeAt ){ changedAt_ = changeAt; }
    void enable(bool enable) { enable_ = enable; }

    const int& id() { return analysisId_; }
    const AnalysisType& type() { return type_; }
    QString typeJson() const;
    const QList<Source *>& sources() const;
    QList<Source *> sources(SourceDirection direction) const;

    const QString& name() const { return name_; }
    const QDateTime& createAt() const { return createdAt_; }
    const QDateTime& changeAt() const { return changedAt_; }
    const bool& enable() const { return enable_; }
    const int& deviceId() const { return deviceId_; }


    void clearSources(SourceDirection direction);

    QString sourcesJson(SourceDirection direction) const;

    void analyses(const QVariantList& analyses);
    const QVariantList& analyses() const;
    void addAnalysis(const QVariantMap& analysis);
    int numberAnalyses();
    QVariantMap analysis(int i) const;

    void analysesJson(const QString& json);
    QString analysesJson();
private:
    QVariantList analyses_;

    int analysisId_;
    int deviceId_;
    AnalysisType type_;

    QList<Source*> sources_;

    QString name_;
    QDateTime changedAt_, createdAt_;
    bool enable_;

};



}
}


#endif // ANALYSISMODEL_H
