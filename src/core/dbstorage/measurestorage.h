#ifndef MEASURESTORAGE_H
#define MEASURESTORAGE_H
#include "dbstorage.h"
#include <singleton.h>

#include "../models/measuremodel.h"

#define CACHE_SIZE_MEASURE_STORAGE 10

class MeasureStorage : public DbStorage, public Singleton<MeasureStorage> {
public:
    enum MeasureTable{
        TABLE_MEASURES
    };

    MeasureStorage();
    QString dbName() const;

    bool saveMeasure();
    bool saveMeasure(const MeasureModel& measure);
    const MeasureModel& openMeasure(const int& measureId) const;


protected:
    QString connectionName() const;

private:

    MeasureModel openMeasureImpl(const int& measureId);
    bool saveMeasureImpl(const MeasureModel& measure);
    bool createTable( const MeasureTable& table );

    void saveCache( const MeasureModel& measure ) const;

    mutable MeasureModel currentMeasure_;

    mutable QMap<int,MeasureModel> cachedMeasures_;

    static QString CONNECTION_NAME_MEASURE;
    static QString TABLE_NAME_MEASURES;
};


#endif // MEASURESTORAGE_H
