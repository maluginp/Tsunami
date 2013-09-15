#include "measurestorage.h"
#include "../models/measuremodel.h"

MeasureStorage::MeasureStorage()
{
}

QString MeasureStorage::dbName() {
    return dbMain;
}

void MeasureStorage::createDbStructure() {
    QStringList columns;
    columns << "measure_id PK";

    createTable( "measures", columns );

}

QString MeasureStorage::connectionName() const {
    return measureConnectionName;
}


bool MeasureStorage::addMeasureImpl(const MeasureModel &measure) {



}


MeasureModel MeasureStorage::openMeasure(const int &measureId) {

}
