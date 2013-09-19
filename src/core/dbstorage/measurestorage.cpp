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

void MeasureStorage::createTable(const MeasureTable &table) {

    if(table == MeasureStorage::TABLE_MEASURES){
        QString querySql = sql("CREATE TABLE IF NOT EXISTS %1 ("
                               "measure_id INTEGER PRIMARY KEY ON CONFLICT REPLACE,"
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

bool MeasureStorage::saveMeasureImpl(const MeasureModel &measure) {
    return false;
}


MeasureModel MeasureStorage::openMeasure(const int &measureId) {
    return MeasureModel();
}
