#ifndef ANALYSISSTORAGE_H
#define ANALYSISSTORAGE_H
#include "dbstorage.h"
#include <singleton.h>
#include "../models/analysismodel.h"

class AnalysisStorage : public DbStorage, public Singleton<AnalysisStorage> {
public:
    enum AnalysisTable{
        TABLE_ANALYSES
    };

    AnalysisStorage();
    QString dbName() const;

    bool saveAnalysis();
    bool saveAnalysis(const AnalysisModel& analysis);

    AnalysisModel openAnalysis(const int& analysisId);

protected:
    QString connectionName() const;
private:
    bool saveAnalysisImpl(const AnalysisModel& analysis);

    AnalysisModel openAnalysisImpl(const int& analysisId);
    bool createTable(const AnalysisTable& table);


    AnalysisModel currentAnalysis_;
    static QString CONNECTION_NAME_ANALYSIS;
    static QString TABLE_NAME_ANALYSES;
};

#endif // ANALYSISSTORAGE_H
