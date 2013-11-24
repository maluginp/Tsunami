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
    configuration.insert("step", 1.0);
    model->addSource( Source("C",SOURCE_MODE_VOLTAGE,SOURCE_DIRECTION_INPUT,SOURCE_METHOD_LINEAR, configuration) );

    configuration.clear();

    configuration.insert("start", 0.0);
    configuration.insert("end",1.0);
    configuration.insert("step", 0.1);
    model->addSource( Source("B",SOURCE_MODE_VOLTAGE,SOURCE_DIRECTION_INPUT,SOURCE_METHOD_LINEAR, configuration) );

    model->header( MeasureHeader("No comment, althought test date =)") );

    QStringList columns;
    columns << "Vc" << "Vb" << "Ve" << "Ib" << "Ic" ;
    model->columns(columns);

    QVector< QVector<double> > data;

    for(double val1=0.0; val1 <= 10.0; val1 += 1.0){
        QVector<double> row;
        for(double val2=0.0; val2 <= 1.0; val2 += 0.1){
            row << val1 << val2 << 0.0 <<  val1+val2 << 2*(val1+val2);
        }
        data.append( row );
    }

    model->data(data);

    model->createAt( QDateTime::currentDateTime() );
    model->changeAt( QDateTime::currentDateTime() );
    model->enable(true);
    model->userId(1);

    if(!saveMeasure( model )){
        // Error
    }

    delete model;


}

MeasureModel *MeasureStorage::openMeasureImpl(int measureId) {
    setLastError( QString() );

    QString sqlQuery;

    sqlQuery = sql("SELECT * FROM %1 WHERE measure_id=:id")
            .arg( TABLE_NAME_MEASURES );

    QSqlQuery q(sqlQuery,db());
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
        measureId = q.lastInsertId().toInt() + 1;

    }
    q.bindValue(":id", measureId);
    q.bindValue(":device_id", measure->deviceId());
    q.bindValue(":name",measure->name());
    q.bindValue(":analysis", measure->typeJson() );
    q.bindValue(":attributes",measure->attrs());
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

QList<MeasureModel> MeasureStorage::findMeasure(const QVariantMap &criteria) {
    // TODO List of measure model
}


}
}
