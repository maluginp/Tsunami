#ifndef ANALYSISSTORAGE_H
#define ANALYSISSTORAGE_H
#include "dbstorage.h"
#include <singleton.h>

namespace tsunami{
namespace db{
class AnalysisModel;

class AnalysisStorage : public DbStorage, public Singleton<AnalysisStorage> {
public:
    enum AnalysisTable{
        TABLE_ANALYSES
    };

    AnalysisStorage();
    QString dbName() const;

    bool saveAnalysis( AnalysisModel* model );
    AnalysisModel* openAnalysis(int analysisId);

    QMap<int,QString> listAnalysis();

protected:
    QString connectionName() const;
private:

    void testData();

    bool saveAnalysisImpl( AnalysisModel* model );
    AnalysisModel* openAnalysisImpl(int analysisId);
    bool createTable(AnalysisTable table);
    QMap<int,QString> listAnalysisImpl();

    static QString CONNECTION_NAME_ANALYSIS;
    static QString TABLE_NAME_ANALYSES;
};


}
}


#endif // ANALYSISSTORAGE_H
