#include "measurestorage.h"
#include "../models/measuremodel.h"

QString MeasureStorage::TABLE_NAME_MEASURES = QString("measures");
QString MeasureStorage::CONNECTION_NAME_MEASURE = QString("measure_connection");

MeasureStorage::MeasureStorage() {

    if(!tableExists( TABLE_NAME_MEASURES )){
        createTable( TABLE_MEASURES );
    }

}

QString MeasureStorage::dbName() const {
    return DbStorage::DBASE_COMMON_NAME;
}

bool MeasureStorage::saveMeasure() {

    bool res = saveMeasureImpl( currentMeasure_ );

    return res;

}

bool MeasureStorage::saveMeasure(const MeasureModel &measure) {
    bool res = saveMeasureImpl( measure );

    return res;
}

QString MeasureStorage::connectionName() const {
    return CONNECTION_NAME_MEASURE;
}

MeasureModel MeasureStorage::openMeasureImpl(const int &measureId) {
    setLastError( QString() );

    if( cachedMeasures_.contains( measureId ) ){
        return cachedMeasures_[measureId];
    }

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


    currentMeasure_ = model;

    saveCache( model );

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

void MeasureStorage::saveCache(const MeasureModel &measure) {
    if( cachedMeasures_.contains( measure.id() ) ){
        cachedMeasures_[measure.id()] = measure;
        return;
    }

    if(cachedMeasures_.size() > CACHE_SIZE_MEASURE_STORAGE){
        cachedMeasures_.erase( cachedMeasures_.begin() );
    }

    cachedMeasures_.insert( measure.id(), measure );



}

bool MeasureStorage::saveMeasureImpl(const MeasureModel &measure) {

    QString sqlQuery;

    MeasureModel model = measure;


    if(!beginTransaction()){
        return false;
    }

    sqlQuery = sql("INSERT OR REPLACE INTO %1(measure_id,project_id,header,"
                   "header_data,data,create_at,change_at,enable) "
                   "VALUES (:measure_id,:project_id,:header,:header_data,:data,"
                   ":create_at,:change_at,:enable)").arg(TABLE_NAME_MEASURES);

    QSqlQuery q(sqlQuery,db());

    if(model.id() == -1){
        int lastInsertId = q.lastInsertId().toInt() + 1;
        model.setId( lastInsertId );
    }

    q.bindValue(":measure_id", measure.id() );
    q.bindValue(":project_id",  measure.projectId());
    q.bindValue(":header", measure.jsonHeader());
    q.bindValue(":header_data",measure.jsonHeaderData());
    q.bindValue(":data",measure.jsonData());
    q.bindValue(":create_at",measure.createAt());
    q.bindValue(":change_at",measure.changeAt());
    q.bindValue(":enable",measure.enable());

    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }

    if(!endTransaction()){
//        roolback();
        return false;
    }

    saveCache( model );

    return true;
}


MeasureModel MeasureStorage::openMeasure(const int &measureId) {
    return openMeasureImpl( measureId );
}
