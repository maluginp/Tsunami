#ifndef MEASURESTORAGE_H
#define MEASURESTORAGE_H
#include "dbstorage.h"
#include <singleton.h>

#include "../models/measuremodel.h"

class MeasureStorage : public DbStorage, public Singleton<MeasureStorage> {
public:
    enum MeasureTable{
        TABLE_MEASURES
    };

    MeasureStorage();
    QString dbName() const;

    bool saveMeasure(const MeasureModel& measure);
    MeasureModel openMeasure(const int& measureId);


protected:
    QString connectionName() const;

private:

    const MeasureModel& openMeasureImpl(const int& measureId) const;
    bool saveMeasureImpl(const MeasureModel& measure);
    void createTable( const MeasureTable& table );

    static QString CONNECTION_NAME_MEASURE;
    static QString TABLE_NAME_MEASURES;
};


#endif // MEASURESTORAGE_H
