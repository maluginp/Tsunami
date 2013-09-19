#include "measurestorage.h"
#include "../models/measuremodel.h"

QString MeasureStorage::TABLE_NAME_MEASURES = QString("measures");
QString MeasureStorage::CONNECTION_NAME_MEASURE = QString("measure_connection");

MeasureStorage::MeasureStorage()
{
}

QString MeasureStorage::dbName() const {
    return DbStorage::DBASE_COMMON_NAME;
}

QString MeasureStorage::connectionName() const {
    return CONNECTION_NAME_MEASURE;
}

const MeasureModel &MeasureStorage::openMeasureImpl(const int &measureId) const {
    setLastError( QString() );

    QString sqlQuery;

    sqlQuery = sql("SELECT * FROM %1 WHERE measure_id=:id")
            .arg( TABLE_NAME_MEASURES );

    QSqlQuery q(sqlQuery,db());
    if(!q.exec() || !q.next()){
        setLastError( q.lastError().text() );
        return MeasureModel();
    }

    QSqlRecord rec( q.record() );

    MeasureModel model;


    model.setId( q.value(rec.indexOf("measure_id")).toInt() );
    model.setProjectId( q.value(rec.indexOf("project_id")).toInt() );
    model.parseJsonHeader( q.value(rec.indexOf("header")).toString() );
    model.parseJsonHeaderData( q.value(rec.indexOf("header_data")).toString());
    model.parseJsonData( q.value(rec.indexOf("data")).toString() );
    model.setCreateAt( q.value(rec.indexOf("create_at")).toDateTime() );
    model.setChangeAt( q.value(rec.indexOf("change_at")).toDateTime() );
    model.setEnable( q.value(rec.indexOf("enable")).toBool() );


    return model;


}

bool MeasureStorage::createTable(const MeasureTable &table) {
    QString sqlQuery;
    if(table == MeasureStorage::TABLE_MEASURES){
        sqlQuery = sql("CREATE TABLE IF NOT EXISTS %1 ("
                       "measure_id INTEGER PRIMARY KEY ON CONFLICT REPLACE,"
                       "project_id INTEGER,"
                       "header TEXT,"
                       "header_data TEXT,"
                       "data TEXT,"
                       "create_at NUMERIC,"
                       "change_at NUMERIC,"
                       "enable NUMERIC,"
                       ")").arg(TABLE_NAME_MEASURES);

    }else{
        return false;
    }

    QSqlQuery q(sqlQuery,db());
    if(!q.exec()){
        setLastError( q.lastError().text() );
        return false;
    }

    return true;
}

bool MeasureStorage::saveMeasureImpl(const MeasureModel &measure) {
    return false;
}


MeasureModel MeasureStorage::openMeasure(const int &measureId) {
    return MeasureModel();
}
