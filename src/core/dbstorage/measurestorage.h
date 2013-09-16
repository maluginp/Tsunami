#ifndef MEASURESTORAGE_H
#define MEASURESTORAGE_H
#include "dbstorage.h"
#include <QVariantMap>
#include <QtSql>
#include <singleton.h>

class MeasureModel;



const QString CONNECTION_NAME_MEASURE = QString("measure_connection");

class MeasureStorage : public DbStorage, public Singleton<MeasureStorage> {
public:
    MeasureStorage();
    QString dbName();

    bool saveMeasure(const MeasureModel& measure);
    MeasureModel openMeasure(const int& measureId);


protected:
    QString connectionName() const;

private:

    MeasureModel openMeasureImpl(const int& measureId);
    bool saveMeasureImpl(const MeasureModel& measure);
    void createTable( const MeasureTable& table );

    enum MeasureTable{
        TABLE_MEASURES
    };

    const QString TABLE_NAME_MEASURES;
};


#endif // MEASURESTORAGE_H
