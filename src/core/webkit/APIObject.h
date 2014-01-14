#ifndef APIOBJECT_H
#define APIOBJECT_H

#include "defines.h"
#include "models/AnalysisModel.h"

class TSUNAMI_EXPORT APIObject : public QObject
{
    Q_OBJECT
public:
    explicit APIObject(int deviceId, QObject *parent = 0);
    void openAnalysis(tsunami::db::AnalysisModel *analysis );
//    void createAnalysis();
public slots:
    QString test();
    int saveAnalysis(const QVariantMap& sources);
signals:
    void openedAnalysis(int analysisId,const QVariantMap& analysis);
    void savedAnalysis( const QList<tsunami::Source>& sources );
public slots:
private:
    tsunami::db::AnalysisModel* analysis_;
    int deviceId_;
};


#endif // APIOBJECT_H
