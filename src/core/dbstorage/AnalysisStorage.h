#ifndef ANALYSISSTORAGE_H
#define ANALYSISSTORAGE_H
#include "DbStorage.h"
#include <singleton.h>

namespace tsunami{
namespace db{
class AnalysisModel;

class TSUNAMI_EXPORT  AnalysisStorage : public DbStorage, public Singleton<AnalysisStorage> {
public:
    enum AnalysisTable{
        TABLE_ANALYSES
    };

    AnalysisStorage();
    QString dbName() const;

    bool saveAnalysis( AnalysisModel* model );
    AnalysisModel* openAnalysis(int analysisId);

    QList<AnalysisModel*> getAnalysesByDeviceId( int deviceId );
    QMap<int,QString> listAnalysis(int deviceId);

protected:
    QString connectionName() const;
    int lastInsertId(const QString& table);
private:

    void testData();

    bool saveAnalysisImpl( AnalysisModel* model );
    AnalysisModel* openAnalysisImpl(int analysisId);
    QList<AnalysisModel*> getAnalysesByDeviceIdImpl( int deviceId );
    bool createTable(AnalysisTable table);
    QMap<int,QString> listAnalysisImpl(int deviceId);

    static QString CONNECTION_NAME_ANALYSIS;
    static QString TABLE_NAME_ANALYSES;
};


}
}


#endif // ANALYSISSTORAGE_H
