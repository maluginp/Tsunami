#include "analysisstorage.h"

QString AnalysisStorage::CONNECTION_NAME_ANALYSIS = QString("connection_analysis");
QString AnalysisStorage::TABLE_NAME_ANALYSES      = QString("analyses");

AnalysisStorage::AnalysisStorage() {
    if(!tableExists( TABLE_NAME_ANALYSES )){
        createTable( TABLE_ANALYSES );
    }
}

QString AnalysisStorage::dbName() const {
    return DBASE_COMMON_NAME;
}

bool AnalysisStorage::saveAnalysis() {
    return saveAnalysisImpl( currentAnalysis_ );
}

bool AnalysisStorage::saveAnalysis(AnalysisModel &analysis) {
    return saveAnalysisImpl(analysis);
}

AnalysisModel AnalysisStorage::openAnalysis(const int &analysisId) {
    return openAnalysisImpl(analysisId);
}

QMap<int, QString> AnalysisStorage::listAnalysis(){
    return listAnalysisImpl();
}

QString AnalysisStorage::connectionName() const {
    return CONNECTION_NAME_ANALYSIS;
}

bool AnalysisStorage::saveAnalysisImpl(AnalysisModel &model) {

    QString sqlQuery;

    if(!beginTransaction()){
        return false;
    }

    sqlQuery=sql("INSERT OR REPLACE INTO %1(analysis_id,device_id,name,type,"
                 "inputs,outputs,createAt,changeAt,enable) "
                 "VALUES (:analysis_id,:device_id,:name,:type,:inputs,:outputs,"
                 ":createAt,:changeAt,:enable)").arg(TABLE_NAME_ANALYSES);

    QSqlQuery q(sqlQuery,db());

    if(model.id() == -1){
        int lastInsertId = q.lastInsertId().toInt() + 1;
        model.setId( lastInsertId );
    }
    q.bindValue(":analysis_id", model.id());
    q.bindValue(":device_id",model.deviceId());
    q.bindValue(":name",model.name());

    switch( model.type() ){
    case AnalysisModel::ANALYSIS_DC: q.bindValue(":type", "dc" ); break;
    case AnalysisModel::ANALYSIS_AC: q.bindValue(":type", "ac" ); break;
    case AnalysisModel::ANALYSIS_TRAN: q.bindValue(":type", "tran" ); break;
    case AnalysisModel::ANALYSIS_UNKNOWN:
    default:
        q.bindValue(":type", "" );
    }

    q.bindValue(":inputs",model.jsonInput());
    q.bindValue(":outputs",model.jsonOutput());
    q.bindValue(":createAt",model.createAt());
    q.bindValue(":changeAt",model.changeAt());
    q.bindValue(":enable",model.enable());

    if(!q.exec()){
        setLastError(q.lastError().text());
        qDebug() << q.lastQuery();
        rollback();
        return false;
    }
    if(!endTransaction()){
        return false;
    }

    saveCache(model);
    return true;
}

AnalysisModel AnalysisStorage::openAnalysisImpl(const int &analysisId) {
    QString sqlQuery;

    if( analysisId == -1 ){
        return AnalysisModel();
    }

    sqlQuery = sql("SELECT * FROM %1 WHERE analysis_id=:id")
            .arg(TABLE_NAME_ANALYSES);

    QSqlQuery q(sqlQuery, db());

    q.bindValue(":id",analysisId);


    if(!q.exec() || !q.next()){
        return AnalysisModel();
    }

    AnalysisModel model;

    QSqlRecord rec(q.record());


    model.setId( q.value(rec.indexOf("analysis_id")).toInt() );
    model.setDeviceId( q.value(rec.indexOf("device_id")).toInt() );
    model.setName( q.value(rec.indexOf("name")).toString() );

    QString type = q.value(rec.indexOf("type")).toString();

    if( type.compare("dc") == 0 ){
        model.setType( AnalysisModel::ANALYSIS_DC );
    } else if(type.compare("ac") == 0){
        model.setType( AnalysisModel::ANALYSIS_AC );
    } else if(type.compare("tran") == 0){
        model.setType( AnalysisModel::ANALYSIS_TRAN );
    } else {
        model.setType( AnalysisModel::ANALYSIS_UNKNOWN );
    }

    model.parseJsonInput(  q.value(rec.indexOf("inputs")).toString() );
    model.parseJsonOutput( q.value(rec.indexOf("outputs")).toString() );

    model.setCreateAt( q.value(rec.indexOf("createAt")).toDateTime() );
    model.setChangeAt( q.value(rec.indexOf("changeAt")).toDateTime() );

    model.setEnable( q.value(rec.indexOf("enable")).toBool() );

    currentAnalysis_ = model;

    saveCache( model );

    return model;
}

bool AnalysisStorage::createTable(const AnalysisStorage::AnalysisTable &table) {
    QString sqlQuery;
    if(table == TABLE_ANALYSES){
        sqlQuery = sql("CREATE TABLE IF NOT EXISTS %1 ("
                       "analysis_id INTEGER PRIMARY KEY ON CONFLICT REPLACE,"
                       "device_id INTEGER,"
                       "name TEXT,"
                       "type TEXT,"
                       "inputs TEXT,"
                       "outputs TEXT,"
                       "createAt NUMERIC,"
                       "changeAt NUMERIC,"
                       "enable NUMERIC)").arg(TABLE_NAME_ANALYSES);
    }

    if(sqlQuery.isEmpty()){
        return false;
    }

    QSqlQuery q(sqlQuery,db());

    if(!q.exec()){
        setLastError(q.lastError().text());
        return false;
    }

    return true;
}

QMap<int, QString> AnalysisStorage::listAnalysisImpl() {
    QString sqlQuery;
    QMap<int,QString> analyses;
    sqlQuery = sql("SELECT analysis_id,name FROM %1").arg(TABLE_NAME_ANALYSES);

    QSqlQuery q(sqlQuery,db());
    if(!q.exec()){
        return QMap<int,QString>();
    }

    while( q.next() ){
        QSqlRecord rec( q.record() );
        analyses.insert( q.value(rec.indexOf("analysis_id")).toInt(),
                         q.value(rec.indexOf("name")).toString() );
    }

    return analyses;
}

void AnalysisStorage::saveCache(const AnalysisModel &analysis) {

}
