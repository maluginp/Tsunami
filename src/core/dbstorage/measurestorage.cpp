#include "measurestorage.h"
#include "../models/measuremodel.h"

namespace tsunami{
namespace db{

QString MeasureStorage::TABLE_NAME_MEASURES = QString("measures");
QString MeasureStorage::CONNECTION_NAME_MEASURE = QString("measure_connection");

#define ITEM(column) q.value(rec.indexOf(column))

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


    model.id(          ITEM("id").toInt()  );
    model.deviceId(    ITEM("device_id").toInt() );
    model.name(        ITEM("name").toString() );
    model.type(        ITEM("analysis").toString());
    model.attrsJson(   ITEM("attributes").toString());
    model.sourcesJson( ITEM("sources").toString());
    model.columnsJson( ITEM("columns").toString());
    model.dataJson(    ITEM("data").toString());
    model.createAt(    ITEM("created_at").toDate());
    model.changeAt(    ITEM("changet_at").toDate());
    model.enable(      ITEM("enable").toBool());
    model.userId(      ITEM("user_id").toInt());


    currentMeasure_ = model;

    saveCache( model );

    return model;


}

bool MeasureStorage::createTable(const MeasureTable &table) {
    QString sqlQuery;
    if(table == MeasureStorage::TABLE_MEASURES){
        sqlQuery = sql("CREATE TABLE IF NOT EXISTS %1 ("
                       "id INTEGER PRIMARY KEY ON CONFLICT REPLACE,"
                       "device_id INTEGER,"
                       "name TEXT,"
                       "analysis TEXT,"
                       "attributes TEXT,"
                       "sources TEXT,"
                       "header TEXT,"
                       "columns TEXT,"
                       "data TEXT,"
                       "user_id INTEGER,"
                       "created_at NUMERIC,"
                       "changed_at NUMERIC,"
                       "enable NUMERIC"
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

void MeasureStorage::saveCache(const MeasureModel &measure) const {
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

    Q_ASSERT(false);
    return false;
    QString sqlQuery;

//    MeasureModel model(measure);


//    if(!beginTransaction()){
//        return false;
//    }

//    sqlQuery = sql("INSERT OR REPLACE INTO %1(,"
//                   ") "
//                   "VALUES (:measure_id,:project_id,:header,:header_data,:data,"
//                   ":create_at,:change_at,:enable)").arg(TABLE_NAME_MEASURES);

//    QSqlQuery q(sqlQuery,db());

//    if(model.id() == -1){
//        int lastInsertId = q.lastInsertId().toInt() + 1;
//        model.setId( lastInsertId );
//    }

//    q.bindValue(":measure_id", model.id() );
//    q.bindValue(":project_id",  model.projectId());
//    q.bindValue(":header", model.jsonHeader());
//    q.bindValue(":header_data",model.jsonHeaderData());
//    q.bindValue(":data",model.jsonData());
//    q.bindValue(":create_at",model.createAt());
//    q.bindValue(":change_at",model.changeAt());
//    q.bindValue(":enable",model.enable());

//    if(!q.exec()){
//        setLastError( q.lastError().text() );
//        rollback();
//        return false;
//    }

//    if(!endTransaction()){
////        roolback();
//        return false;
//    }

//    saveCache( model );

//    return true;
}


MeasureModel MeasureStorage::openMeasure(const int &measureId) {
    return openMeasureImpl( measureId );
}

QList<MeasureModel> MeasureStorage::findMeasures(TypeAnalysis type, DeviceType device) {


}

}
}
