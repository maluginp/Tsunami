#include "parameterstorage.h"

const QString ParameterStorage::TABLE_NAME_PARAMETERS = QString("parameters");
const QString ParameterStorage::TABLE_NAME_LIBRARIES  = QString("libraries");

ParameterStorage::ParameterStorage()
{
}

QString ParameterStorage::dbName() const {
    return DbStorage::dbMain;

}

QString ParameterStorage::connectionName() const {
    return CONNECTION_NAME_PARAMETER;
}

void ParameterStorage::saveLibraryImpl(const LibraryModel &library) {

    QString sqlQuery;

//    sqlQuery = sql("INSERT OR REPLACE INTO %1("
//                   "library_id")
}

void ParameterStorage::createTable(const ParameterStorage::ParameterTable &table) {
    QString sqlQuery("");
    if(table == ParameterStorage::TABLE_LIBRARIES){
        sqlQuery = sql( "CREATE TABLE IF NOT EXISTS %1 ("
                        "library_id INTEGER PRIMARY KEY,"
                        "name TEXT,"
                        "project_id INTEGER,"
                        "user_id INTEGER,"
                        "create_at NUMERIC,"
                        "change_at NUMERIC,"
                        "enable NUMERIC"
                        ")").arg(TABLE_NAME_LIBRARIES);

    }else if(table == ParameterStorage::TABLE_PARAMETERS){
        sqlQuery = sql( "CREATE TABLE IF NOT EXISTS %1 ("
                        "param_id INTEGER PRIMARY KEY,"
                        "name TEXT,"
                        "initial REAL,"
                        "minimum REAL,"
                        "maximum REAL,"
                        "library_id INTEGER"
                        ")").arg(TABLE_NAME_PARAMETERS);
    }

}
