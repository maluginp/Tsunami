#ifndef MEASURESTORAGE_H
#define MEASURESTORAGE_H

#include <singleton.h>
#include "DbStorage.h"
#include "models/MeasureModel.h"
#include "defines.h"

#define CACHE_SIZE_MEASURE_STORAGE 10
namespace tsunami{
namespace db{

class MeasureStorage : public DbStorage, public Singleton<MeasureStorage> {
public:
    enum MeasureTable{
        TABLE_MEASURES
    };

    MeasureStorage();
    QString dbName() const;

    bool saveMeasure(MeasureModel* measure);
    MeasureModel* openMeasure(int measureId);

    QList<MeasureModel *> findMeasures( const QVariantMap& criteria );

    QList<MeasureModel*> getMeasuresByDeviceId( int deviceId );
    QList<MeasureModel*> getMeasures(const QList<int>& measureIds );

    QMap<int,QString> getMeasuresIdByDeviceId( int deviceId );

protected:
    QString connectionName() const;
    int lastInsertId(const QString& table);


private:

    void testData();

    QList<MeasureModel*> getMeasuresByDeviceIdImpl(int deviceId);
    MeasureModel* openMeasureImpl(int measureId);
    bool saveMeasureImpl(MeasureModel* measure);
    bool createTable( const MeasureTable& table );

    static QString CONNECTION_NAME_MEASURE;
    static QString TABLE_NAME_MEASURES;
};

}
}
#endif // MEASURESTORAGE_H
