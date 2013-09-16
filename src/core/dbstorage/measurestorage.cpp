#include "measurestorage.h"
#include "../models/measuremodel.h"

const QString MeasureStorage::TABLE_NAME_MEASURES = QString("measures");

MeasureStorage::MeasureStorage()
{
}

QString MeasureStorage::dbName() {
    return DbStorage::dbMain;
}

QString MeasureStorage::connectionName() const {
    return CONNECTION_NAME_MEASURE;
}

void MeasureStorage::createTable(const MeasureTable &table) {

    if(table == MeasureStorage::TABLE_MEASURES){
        QString querySql = sql("CREATE TABLE IF NOT EXISTS %1 ("
                               "measure_id INTEGER PRIMARY KEY,"
                               "project_id INTEGER,"
                               "header TEXT,"
                               "header_data TEXT,"
                               "data TEXT,"
                               "create_at NUMERIC,"
                               "change_at NUMERIC,"
                               "enable NUMERIC,"
                               ")").arg(TABLE_NAME_MEASURES);

    }

}
QString MeasureStorage::getTableMeasures() const
{
    return tableMeasures;
}



bool MeasureStorage::addMeasureImpl(const MeasureModel &measure) {



}


MeasureModel MeasureStorage::openMeasure(const int &measureId) {

}
