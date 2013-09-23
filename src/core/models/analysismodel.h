#ifndef ANALYSISMODEL_H
#define ANALYSISMODEL_H
#include <Model.h>
#include <QDateTime>
#include "analysisitems.h"

//typedef QList<IAnalysisItem*> AnalysisItems;

class AnalysisModel : public Model {
    friend class AnalysisStorage;
public:
    enum AnalysisType{
        ANALYSIS_UNKNOWN,
        ANALYSIS_DC,
        ANALYSIS_AC,
        ANALYSIS_TRAN
    };

    AnalysisModel();

    const int& id() const     { return analysisId_; }
    const int& deviceId() const { return deviceId_; }
    const QString& name() const { return name_; }
    const AnalysisType& type() const { return type_; }
    const QList<IAnalysisItem*>& inputs() const { return inputs_; }
    const QList<IAnalysisItem*>& outputs() const { return outputs_; }
    const QDateTime& createAt() const { return createAt_; }
    const QDateTime& changeAt() const { return changeAt_; }
    const bool& enable() const { return enable_; }

    void removeItemInput( const QString& nameItem );
    void removeItemOutput(const QString& nameItem );

    AnalysisModel& setId( const int& analysisId );
    AnalysisModel& setDeviceId( const int& deviceId );
    AnalysisModel& setName( const QString& name);
    AnalysisModel& setType( const AnalysisType& type);
    AnalysisModel& setType( const QString& type);
    AnalysisModel& setInputs( const QList<IAnalysisItem*>& inputs );
    AnalysisModel& addInput( IAnalysisItem* input );
    AnalysisModel& setOutputs( const QList<IAnalysisItem*>& outputs );
    AnalysisModel& addOutput ( IAnalysisItem* output );
    AnalysisModel& setCreateAt( const QDateTime& createAt );
    AnalysisModel& setChangeAt( const QDateTime& changeAt );
    AnalysisModel& setEnable( const bool& enable );

protected:
    void parseJsonInput( const QString& json );
    void parseJsonOutput( const QString& json );
    QString jsonInput();
    QString jsonOutput();
private:
    bool inputExists( const QString& name );
    bool outputExists( const QString& name );
    int analysisId_;
    int deviceId_;
    QString name_;
    AnalysisType type_;
    QList<IAnalysisItem *> inputs_;
    QList<IAnalysisItem *> outputs_;
    QDateTime createAt_;
    QDateTime changeAt_;
    bool enable_;
};

#endif // ANALYSISMODEL_H
