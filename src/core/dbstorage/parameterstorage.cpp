#include "parameterstorage.h"
#include "../models/librarymodel.h"

QString ParameterStorage::TABLE_NAME_PARAMETERS = QString("parameters");
QString ParameterStorage::TABLE_NAME_LIBRARIES  = QString("libraries");
QString ParameterStorage::CONNECTION_NAME_PARAMETER = QString("parameter_connection");


ParameterStorage::ParameterStorage() : DbStorage(0)
{
}

QString ParameterStorage::dbName() const {
    return DbStorage::DBASE_COMMON_NAME;

}

QString ParameterStorage::connectionName() const {
    return CONNECTION_NAME_PARAMETER;
}

void ParameterStorage::saveLibraryImpl(const LibraryModel &library) {

    QString sqlQuery;

    //! Start commit

    sqlQuery = sql("INSERT INTO %1( name, project_id, user_id, create_at, change_at, enable) "
                   "       VALUES (:name,:project_id,:user_id,:create_at,:change_at,:enable)")
            .arg(TABLE_NAME_LIBRARIES);

    QSqlQuery q( sqlQuery, db() );




    int lastInsertId=0;

    sqlQuery = sql("INSERT INTO %1( name, initial, minimum, maximum, library_id)"
                   "        VALUES(:name,:initial,:minimum,:maximum,:library_id")
            .arg(TABLE_NAME_PARAMETERS);

    saveToCache( library );
}

LibraryModel ParameterStorage::openLibraryImpl(const int &libraryId) {
   QString sqlQuery;
   LibraryModel library;
   sqlQuery = sql("SELECT * FROM %1 WHERE library_id=:library_id").arg(TABLE_NAME_LIBRARIES);

   lastLibrary_ = library;
   saveToCache( library );

   return library;
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

void ParameterStorage::saveToCache(const LibraryModel &library) {

    if(cachedLibraries_.size() <= CACHE_SIZE_PARAMETER_STORAGE){
        cachedLibraries_.insert( library.id(), library );
    }else{
        //! Удаляем первый элемент кэша
        cachedLibraries_.erase( cachedLibraries_.begin() );
        cachedLibraries_.insert( library.id(), library );
    }

}
