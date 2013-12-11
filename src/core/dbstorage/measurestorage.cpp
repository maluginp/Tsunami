#include "measurestorage.h"
#include "../models/measuremodel.h"

namespace tsunami{
namespace db{

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

bool MeasureStorage::saveMeasure(MeasureModel *measure) {
    bool res = saveMeasureImpl( measure );

    return res;
}

QString MeasureStorage::connectionName() const {
    return CONNECTION_NAME_MEASURE;
}

int MeasureStorage::lastInsertId(const QString &table) {
    QString sqlQuery;
    if(table.compare(TABLE_NAME_MEASURES) == 0){
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

void MeasureStorage::testData() {
    MeasureModel* model = new MeasureModel();
    model->id(1);
    model->deviceId(1);
    model->name( "Test measures" );

    // BJT

    model->type(ANALYSIS_DC);

    // Create Sources
    model->addSource(Source("E",SOURCE_MODE_GND,SOURCE_DIRECTION_INPUT,SOURCE_METHOD_CONST));
    QVariantMap configuration;
    configuration.insert("start", 0.0);
    configuration.insert("end",10.0);
    configuration.insert("step", 5.0);
    configuration.insert("number", 2);

    model->addSource( Source("C",SOURCE_MODE_VOLTAGE,SOURCE_DIRECTION_INPUT,SOURCE_METHOD_LINEAR, configuration) );

    configuration.clear();

    configuration.insert("start", 0.0);
    configuration.insert("end",1.0);
    configuration.insert("step", 0.1);
    configuration.insert("number", 1);
    model->addSource( Source("B",SOURCE_MODE_VOLTAGE,SOURCE_DIRECTION_INPUT,SOURCE_METHOD_LINEAR, configuration) );

    model->header( MeasureHeader("No comment, althought test date =)") );

    // OUTPUT

    model->addSource( Source("B",SOURCE_MODE_CURRENT,SOURCE_DIRECTION_OUTPUT,SOURCE_METHOD_CONST) );
    model->addSource( Source("C",SOURCE_MODE_CURRENT,SOURCE_DIRECTION_OUTPUT,SOURCE_METHOD_CONST) );


    QStringList columns;
    columns << "Vb" << "Vc" << "Ve" << "Ib" << "Ic" ;
    model->columns(columns);

    QVector< QVector<double> > data;

    for(double val1=0.0; val1 <= 10.0; val1 += 5.0){

        for(double val2=0.0; val2 <= 1.0; val2 += 0.1){
            QVector<double> row;
            row << val2 << val1 << 0.0 <<  val1+val2 << 2*(val1+val2);
            data.append( row );
        }

    }

    model->data(data);
    QVariantMap attributes;
    attributes.insert("w",1e-6);
    attributes.insert("l",1e-3);
    model->attrs( attributes );

    model->createAt( QDateTime::currentDateTime() );
    model->changeAt( QDateTime::currentDateTime() );
    model->enable(true);
    model->userId(1);

    if(!saveMeasure( model )){
        // Error
    }

    delete model;


}

QList<MeasureModel *> MeasureStorage::getMeasuresByDeviceIdImpl(int deviceId) {

    setLastError( QString() );

    QList<MeasureModel*> measures;

    QString sqlQuery;

    sqlQuery = sql("SELECT * FROM %1 WHERE device_id=:device_id")
            .arg( TABLE_NAME_MEASURES );

    QSqlQuery q(sqlQuery,db());
    q.bindValue(":device_id", deviceId);

    if(!q.exec()){
        setLastError( q.lastError().text() );
        return QList<MeasureModel*>();
    }

    while(q.next()){

        QSqlRecord rec( q.record() );
        MeasureModel* model = new MeasureModel();


        model->id(          ITEM("id").toInt()  );
        model->deviceId(    ITEM("device_id").toInt() );
        model->name(        ITEM("name").toString() );
        model->type(        ITEM("analysis").toString());
        model->attrsJson(   ITEM("attributes").toString());
        model->sourcesJson( ITEM("sources").toString());
        model->columnsJson( ITEM("columns").toString());
        model->dataJson(    ITEM("data").toString());
        model->createAt(    ITEM("created_at").toDateTime());
        model->changeAt(    ITEM("changet_at").toDateTime());
        model->enable(      ITEM("enable").toBool());
        model->userId(      ITEM("user_id").toInt());

        measures.append( model );
    }

    return measures;

}

MeasureModel *MeasureStorage::openMeasureImpl(int measureId) {
    setLastError( QString() );

    QString sqlQuery;

    sqlQuery = sql("SELECT * FROM %1 WHERE id=:id")
            .arg( TABLE_NAME_MEASURES );

    QSqlQuery q(sqlQuery,db());

    q.bindValue(":id",measureId);

    if(!q.exec() || !q.next()){
        setLastError( q.lastError().text() );
        return NULL;
    }

    QSqlRecord rec( q.record() );
    MeasureModel* model = new MeasureModel();


    model->id(          ITEM("id").toInt()  );
    model->deviceId(    ITEM("device_id").toInt() );
    model->name(        ITEM("name").toString() );
    model->type(        ITEM("analysis").toString());
    model->attrsJson(   ITEM("attributes").toString());
    model->sourcesJson( ITEM("sources").toString());
    model->columnsJson( ITEM("columns").toString());
    model->dataJson(    ITEM("data").toString());
    model->createAt(    ITEM("created_at").toDateTime());
    model->changeAt(    ITEM("changet_at").toDateTime());
    model->enable(      ITEM("enable").toBool());
    model->userId(      ITEM("user_id").toInt());


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

#ifdef QT_DEBUG
    testData();
#endif

    return true;
}

bool MeasureStorage::saveMeasureImpl(MeasureModel *measure) {

     QString sqlQuery;
     sqlQuery = sql("INSERT OR REPLACE INTO %1("
                    "id,device_id,name,analysis,attributes,sources,header,"
                    "columns,data,user_id,created_at,changed_at,enable) "
                    "VALUES("
                    ":id,:device_id,:name,:analysis,:attributes,:sources,:header,"
                    ":columns,:data,:user_id,:created_at,:changed_at,:enable) "
                    ).arg(TABLE_NAME_MEASURES);

    int measureId = measure->id();
    if(!beginTransaction()){
        return false;
    }

    QSqlQuery q(sqlQuery,db());

    if(measureId == -1){
        measureId = lastInsertId(TABLE_NAME_MEASURES) + 1;

    }
    q.bindValue(":id", measureId);
    q.bindValue(":device_id", measure->deviceId());
    q.bindValue(":name",measure->name());
    q.bindValue(":analysis", measure->typeJson() );
    q.bindValue(":attributes",measure->attrsJson());
    q.bindValue(":sources", measure->sourcesJson());
    q.bindValue(":header",measure->headerJson());
    q.bindValue(":columns", measure->columnsJson());
    q.bindValue(":data",measure->dataJson());
    q.bindValue(":user_id", measure->userId());
    q.bindValue(":created_at", measure->createAt());
    q.bindValue(":changed_at",measure->changeAt());
    q.bindValue(":enable",measure->enable());

    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }

    if(!endTransaction()){
////        roolback();
        return false;
    }

    measure->id( measureId );

//    saveCache( model );

    return true;
}


MeasureModel *MeasureStorage::openMeasure(int measureId) {
    return openMeasureImpl( measureId );
}

QList<MeasureModel*> MeasureStorage::findMeasures(const QVariantMap &criteria) {
    QList<MeasureModel*> measures;

    QString sqlQuery = sql("SELECT * FROM %1 WHERE ").arg(TABLE_NAME_MEASURES);
    QStringList criteriaSql;

    int deviceId = -1;
    QString type;

    if( criteria.contains("device") ) {
        criteriaSql.append( "device_id=:device_id" );
        deviceId = criteria.value( "device", -1 ).toInt();
    }

    if( criteria.contains("analysis") ) {
        criteriaSql.append( "analysis=:type" );
        type = criteria.value("analysis",QString()).toString();
    }

    if(criteriaSql.isEmpty()){
        return measures;
    }
    sqlQuery.append( criteriaSql.join(" AND ") );

    QSqlQuery q(sqlQuery,db());

    if( deviceId != -1 ){ q.bindValue(":device_id",deviceId); }
    if( !type.isEmpty() ){  q.bindValue(":type",type); }
    if(!q.exec()){
        setLastError( q.lastError().text() );
        return measures;
    }
    while(q.next()){
        QSqlRecord rec( q.record() );
        MeasureModel* model = new MeasureModel();

        model->id(          ITEM("id").toInt()  );
        model->deviceId(    ITEM("device_id").toInt() );
        model->name(        ITEM("name").toString() );
        model->type(        ITEM("analysis").toString());
        model->attrsJson(   ITEM("attributes").toString());
        model->sourcesJson( ITEM("sources").toString());
        model->columnsJson( ITEM("columns").toString());
        model->dataJson(    ITEM("data").toString());
        model->createAt(    ITEM("created_at").toDateTime());
        model->changeAt(    ITEM("changet_at").toDateTime());
        model->enable(      ITEM("enable").toBool());
        model->userId(      ITEM("user_id").toInt());

        measures.append( model );
    }

    return measures;
}

QList<MeasureModel *> MeasureStorage::getMeasuresByDeviceId(int deviceId) {
    return getMeasuresByDeviceIdImpl(deviceId);
}

QList<MeasureModel *> MeasureStorage::getMeasures(const QList<int> &measureIds) {
    QList<MeasureModel*> measures;

    foreach(int measureId,measureIds){
        measures.append( openMeasure(measureId) );
    }

    return measures;
}

QMap<int, QString> MeasureStorage::getMeasuresIdByDeviceId(int deviceId) {
    QList<MeasureModel *> measures = getMeasuresByDeviceId( deviceId );
    QMap<int,QString> measuresId;

    int numberMeasures = measures.count();
    for(int i=0; i < numberMeasures; ++i){
        measuresId.insert(measures[i]->id(), measures[i]->name());
        delete measures[i];
    }

    return measuresId;
}


}
}
