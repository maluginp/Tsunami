#include "analysisstorage.h"
#include "models/analysismodel.h"
#include <QDebug>
namespace tsunami {
namespace db{

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

bool AnalysisStorage::saveAnalysis(AnalysisModel *model) {
    return saveAnalysisImpl(model);
}

AnalysisModel *AnalysisStorage::openAnalysis(int analysisId) {
    return openAnalysisImpl(analysisId);
}

QList<AnalysisModel *> AnalysisStorage::getAnalysesByDeviceId(int deviceId){
    return getAnalysesByDeviceIdImpl( deviceId );
}

QMap<int, QString> AnalysisStorage::listAnalysis(int deviceId) {
    return listAnalysisImpl(deviceId);
}

QString AnalysisStorage::connectionName() const {
    return CONNECTION_NAME_ANALYSIS;
}

int AnalysisStorage::lastInsertId(const QString &table) {
    QString sqlQuery;
    if(table.compare(TABLE_NAME_ANALYSES) == 0){
        sqlQuery = sql("SELECT MAX(id) FROM %1").arg(table);

        QSqlQuery q( sqlQuery, db() );
        if(q.exec() && q.next()){
            bool ok;
            int id = q.value(0).toInt(&ok);
            if(ok){
                return id;
            }
        }

    }

    return -1;
}

void AnalysisStorage::testData() {
    // Test Analysis
    AnalysisModel* model = new AnalysisModel();
    model->id( 1 );
    model->type("ac");
    model->type(ANALYSIS_DC);
    model->deviceId(1);
    model->name("Test DC analysis");
    model->enable(true);

    // Sources;

    model->addSource(Source("E",SOURCE_MODE_GND,SOURCE_DIRECTION_INPUT,SOURCE_METHOD_CONST));
    QVariantMap configuration;
    configuration.insert("start", 0.0);
    configuration.insert("end",10.0);
    configuration.insert("step", 1.0);
    configuration.insert("number",1);
    model->addSource( Source("C",SOURCE_MODE_VOLTAGE,SOURCE_DIRECTION_INPUT,SOURCE_METHOD_LINEAR, configuration) );

    configuration.clear();

    configuration.insert("start", 0.0);
    configuration.insert("end",1.0);
    configuration.insert("step", 0.1);
    configuration.insert("number",2);
    model->addSource( Source("B",SOURCE_MODE_VOLTAGE,SOURCE_DIRECTION_INPUT,SOURCE_METHOD_LINEAR, configuration) );

    model->addSource(Source("B",SOURCE_MODE_CURRENT,SOURCE_DIRECTION_OUTPUT));
    model->addSource(Source("C",SOURCE_MODE_CURRENT,SOURCE_DIRECTION_OUTPUT));


    if(!saveAnalysis( model )){
        Q_ASSERT(false);
    }

    delete model;

    return;

}

bool AnalysisStorage::saveAnalysisImpl(AnalysisModel *model) {
    setLastError( QString() );

    int analysisId = model->id();

    QString sqlQuery;

    if(!beginTransaction()){
        return false;
    }

    sqlQuery = sql("INSERT OR REPLACE INTO %1(id,device_id,name,type,sources,createdAt,changedAt,enable) "
                   "VALUES(:id,:device,:name,:type,:sources,:createdAt,:changedAt,:enable)")
            .arg(TABLE_NAME_ANALYSES);

    QSqlQuery q(sqlQuery,db());

    if( model->id() == -1 ){
        analysisId = lastInsertId(TABLE_NAME_ANALYSES) +1;
    }

    q.bindValue(":id", analysisId);

    q.bindValue(":device",model->deviceId());
    q.bindValue(":name", model->name());
    q.bindValue(":type",model->typeJson());
    q.bindValue(":sources",model->sourcesJson());
    q.bindValue(":createdAt",model->createAt());
    q.bindValue(":changedAt",model->changeAt());
    q.bindValue(":enable",model->enable());

    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }

    if(!endTransaction()){
        rollback();
        return false;
    }

    model->id( analysisId );

    return true;
}

AnalysisModel *AnalysisStorage::openAnalysisImpl(int analysisId) {
    setLastError( QString() );

    QString sqlQuery;

    AnalysisModel* model = new AnalysisModel();

    sqlQuery = sql("SELECT * FROM %1 WHERE id=:id").arg(TABLE_NAME_ANALYSES);

    QSqlQuery q(sqlQuery,db());
    q.bindValue(":id",analysisId);

    if(!q.exec() || !q.next()){
        setLastError( q.lastError().text() );
//        Q_ASSERT(false);
        return NULL;
    }

    QSqlRecord rec(q.record());

    model->id(          ITEM("id").toInt() );
    model->deviceId(    ITEM("device_id").toInt());
    model->name(        ITEM("name").toString() );
    model->sourcesJson( ITEM("sources").toString() );
    model->type(        ITEM("type").toString() );
    model->createAt(    ITEM("createdAt").toDateTime() );
    model->changeAt(    ITEM("changedAt").toDateTime());
    model->enable(      ITEM("enable").toBool() );

    if(model->id() == -1){
        Q_ASSERT(false);
        return NULL;
    }


    return model;
}

QList<AnalysisModel *> AnalysisStorage::getAnalysesByDeviceIdImpl(int deviceId) {
    setLastError( QString() );

    QString sqlQuery;

    sqlQuery = sql("SELECT * FROM %1 WHERE device_id=:device_id").arg(TABLE_NAME_ANALYSES);

    QSqlQuery q(sqlQuery,db());
    q.bindValue(":device_id",deviceId);

    if(!q.exec()){
        setLastError( q.lastError().text() );
        Q_ASSERT(false);
        return QList<AnalysisModel *>();
    }
    QList<AnalysisModel*> analyses;
    AnalysisModel* model;
    while(q.next()){

        QSqlRecord rec(q.record());

        model = new AnalysisModel();
        model->id(          ITEM("id").toInt() );
        model->deviceId(    ITEM("device_id").toInt());
        model->name(        ITEM("name").toString() );
        model->sourcesJson( ITEM("sources").toString() );
        model->type(        ITEM("type").toString() );
        model->createAt(    ITEM("createdAt").toDateTime() );
        model->changeAt(    ITEM("changedAt").toDateTime());
        model->enable(      ITEM("enable").toBool() );

        analyses.append( model );
    }


    return analyses;
}

bool AnalysisStorage::createTable(AnalysisTable table) {
    QString sqlQuery;
    if(table == AnalysisStorage::TABLE_ANALYSES){
        sqlQuery = sql("CREATE TABLE IF NOT EXISTS %1 ("
                       "id INTEGER PRIMARY KEY ON CONFLICT REPLACE,"
                       "name TEXT,"
                       "device_id INTEGER,"
                       "type TEXT,"
                       "sources TEXT,"
                       "createdAt NUMERIC,"
                       "changedAt NUMERIC,"
                       "enable NUMERIC)"
                       ).arg(TABLE_NAME_ANALYSES);
    }else{
        return false;
    }


    QSqlQuery q(sqlQuery,db());
    if(!q.exec()){

        setLastError( q.lastError().text() );
        return false;
    }

//#ifdef QT_DEBUG
    testData();
//#endif

    return true;
}

QMap<int, QString> AnalysisStorage::listAnalysisImpl(int deviceId) {
    QMap<int, QString> items;
    QString sqlQuery;
    sqlQuery = sql("SELECT * FROM %1 WHERE device_id=:device_id").arg(TABLE_NAME_ANALYSES);

    QSqlQuery q(sqlQuery,db());
    q.bindValue(":device_id", deviceId);

    if(!q.exec()){
        setLastError( q.lastError().text() );
        return QMap<int, QString>();
    }


    while(q.next()){
        QSqlRecord rec(q.record());
        items.insert( ITEM("id").toInt(), ITEM("name").toString() );
    }

    return items;
}



}
}

