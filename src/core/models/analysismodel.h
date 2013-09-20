#ifndef ANALYSISMODEL_H
#define ANALYSISMODEL_H

#include <Model.h>

class AnalysisModel : public Model
{
public:
    enum AnalysisType{
        ANALYSIS_UNKNOWN,
        ANALYSIS_DC,
        ANALYSIS_AC,
        ANALYSIS_TRAN
    };

    AnalysisModel();

    const int& id() const     { return analysisId_; }
    const int& projectId() const { return projectId_; }
    const QString& name() const { return name_; }
    const AnalysisType& type() const { return type_; }
    const QList<IAnalysisItem*>& inputs() const { return inputs_; }
    const QList<IAnalysisItem*>& outputs() const { return outputs_; }
    const QDateTime& createAt() const { return createAt_; }
    const QDateTime& changeAt() const { return changeAt_; }
    const bool& enable() const { return enable_; }

    AnalysisModel& setId( const int& analysisId );
    AnalysisModel& setProjectId( const int& projectId );
    AnalysisModel& setName( const QString& name);
    AnalysisModel& setType( const AnalysisType& type);
    AnalysisModel& setInputs( const QList<IAnalysisItem*>& inputs );
    AnalysisModel& addInput( IAnalysisItem* input );
    AnalysisModel& setOutputs( const QList<IAnalysisItem*>& outputs );
    AnalysisModel& addOutput ( IAnalysisItem* output );
    AnalysisModel& setCreateAt( const QDateTime& createAt );
    AnalysisModel& setChangeAt( const QDateTime& changeAt );
    AnalysisModel& setEnable( const bool& enable );

private:
    int analysisId_;
    int projectId_;
    QString name_;
    AnalysisType type_;
    QList<IAnalysisItem *> inputs_;
    QList<IAnalysisItem *> outputs_;
    QDateTime createAt_;
    QDateTime changeAt_;
    bool enable_;
};

enum AnalysisItemType{
    ANALYSIS_ITEM_NONE,
    ANALYSIS_ITEM_CONST,
    ANALYSIS_ITEM_SWEEP,
    ANALYSIS_ITEM_FUNC
};

class IAnalysisItem{
public:
    void parseJson( const QString& json ) = 0;

};

#endif // ANALYSISMODEL_H
