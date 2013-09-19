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

bool ParameterStorage::saveLibrary() {
    bool res = saveLibraryImpl( lastLibrary_ );
    return res;
}

bool ParameterStorage::saveLibrary(const LibraryModel &library) {
    bool res = saveLibraryImpl( library );
    return res;
}

bool ParameterStorage::addParameterToLibrary(const ParameterModel &parameter) {
    return addParameterToLibraryImpl( parameter );
}

LibraryModel ParameterStorage::openLibrary(const int &libraryId) {
    return openLibraryImpl(libraryId);
}

void ParameterStorage::setCurrentLibrary(const int &libraryId) {
    if(lastLibrary_.id() == libraryId){
        return;
    }

    if( cachedLibraries_.contains(libraryId) ){
        lastLibrary_ = cachedLibraries_[libraryId];
    }else{
        lastLibrary_ = openLibrary( libraryId );
    }
}

QString ParameterStorage::connectionName() const {
    return CONNECTION_NAME_PARAMETER;
}

bool ParameterStorage::saveLibraryImpl(const LibraryModel &library) {

    QString sqlQuery;
    int lastInsertId;
    lastLibrary_ = library;

    //! Start commit
    if(!beginTransaction()){
        return false;
    }

    sqlQuery = sql("INSERT OR REPLACE INTO %1( library_id, name, project_id, user_id, create_at, change_at, enable) "
                   "       VALUES (:library_id,:name,:project_id,:user_id,:create_at,:change_at,:enable)")
            .arg(TABLE_NAME_LIBRARIES);


    QSqlQuery q( sqlQuery, db() );

    q.bindValue(":name",       lastLibrary_.name());
    q.bindValue(":project_id", lastLibrary_.projectId() );
    q.bindValue(":user_id",    lastLibrary_.userId() );
    q.bindValue(":create_at",  lastLibrary_.createAt());
    q.bindValue(":change_at",  lastLibrary_.changeAt());
    q.bindValue(":enable",     lastLibrary_.enable());

    //! TODO: strong check
    if(lastLibrary_.id() == -1){
        lastInsertId = q.lastInsertId().toInt();
        lastLibrary_.setId(lastInsertId);
    }

    q.bindValue(":library_id", lastLibrary_.id() );

    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }



    sqlQuery = sql("INSERT OR REPLACE INTO %1(param_id, name, initial, minimum, maximum, library_id)"
                   "        VALUES(:param_id,:name,:initial,:minimum,:maximum,:library_id")
            .arg(TABLE_NAME_PARAMETERS);

    foreach(ParameterModel parameter,lastLibrary_.parameters()){
        q = QSqlQuery( sqlQuery, db() );

        if(parameter.id() != -1 ){
            lastInsertId = -1;
            parameter.setId(  lastInsertId  );

            lastLibrary_.setParameter( parameter.name(), parameter );
        }

        q.bindValue(":name",parameter.name());
        q.bindValue(":initial",parameter.initial());
        q.bindValue(":minimum",parameter.initial());
        q.bindValue(":maximum",parameter.maximum());
        q.bindValue(":library_id",parameter.libraryId());
        q.bindValue(":param_id",parameter.id());
        if(!q.exec()){
            setLastError( q.lastError().text() );
            rollback();
            return false;
        }

    }

    if(!endTransaction()){
        rollback();
        return false;
    }


    saveToCache( lastLibrary_ );
    return true;
}

LibraryModel ParameterStorage::openLibraryImpl(const int &libraryId) const {
    setLastError(QString());
    QString sqlQuery;
    LibraryModel library;
    sqlQuery = sql("SELECT * FROM %1 WHERE library_id=:library_id").arg(TABLE_NAME_LIBRARIES);

    QSqlQuery q(sqlQuery,db());

    q.bindValue(":library_id",libraryId);

    if(!q.exec() || !q.next()){
        setLastError( q.lastError().text() );
        return LibraryModel();
    }

    QSqlRecord rec(q.record());

    library.setId(        q.value(rec.indexOf("library_id" )).toInt()    );
    library.setName(      q.value(rec.indexOf("name")).toInt()           );
    library.setProjectId( q.value(rec.indexOf("project_id")).toInt()     );
    library.setUserId(    q.value(rec.indexOf("user_id")).toInt()        );
    library.setCreateAt(  q.value(rec.indexOf("create_at")).toDateTime() );
    library.setChangeAt(  q.value(rec.indexOf("change_at")).toDateTime() );
    library.setEnable(    q.value(rec.indexOf("enable")).toBool()        );

    if(library.id() == -1){
        return LibraryModel();
    }

    sqlQuery =sql("SELECT * FROM %1 WHERE library_id=:library_id").arg(TABLE_NAME_PARAMETERS);

    q = QSqlQuery(sqlQuery,db());
    q.bindValue( ":library_id", library.id() );

    if(!q.exec()){
        setLastError( q.lastError().text() );
        return LibraryModel();
    }

    while( q.next() ){
        rec(q.record());
        ParameterModel parameter;
        parameter.setId( q.value( rec.indexOf("param_id")).toInt() )
                .setName( q.value( rec.indexOf("name") ).toString() )
                .setInitial( q.value( rec.indexOf("initial")).toDouble() )
                .setMinimum( q.value( rec.indexOf("minimum")).toDouble() )
                .setMaximum( q.value( rec.indexOf("maximum")).toDouble() )
                .setLibraryId( q.value( rec.indexOf("libraryId") ).toInt() );

        if(parameter.id() != -1){
            library.parameters_.append( parameter );
        }
    }



    lastLibrary_ = library;
    saveToCache( library );

    return library;
}

void ParameterStorage::addParameterToLibraryImpl(const ParameterModel &parameter) const {
    if(lastLibrary_.parameterExists( parameter.name() )){
        return;
    }

    lastLibrary_.parameters_.append( parameter );
    saveLibraryImpl( lastLibrary_ );
    saveToCache( lastLibrary_);

}

bool ParameterStorage::createTable(const ParameterStorage::ParameterTable &table) {
    QString sqlQuery("");
    if(table == ParameterStorage::TABLE_LIBRARIES){
        sqlQuery = sql( "CREATE TABLE IF NOT EXISTS %1 ("
                        "library_id INTEGER PRIMARY KEY ON CONFLICT REPLACE,"
                        "name TEXT,"
                        "project_id INTEGER,"
                        "user_id INTEGER,"
                        "create_at NUMERIC,"
                        "change_at NUMERIC,"
                        "enable NUMERIC"
                        ")").arg(TABLE_NAME_LIBRARIES);

    }else if(table == ParameterStorage::TABLE_PARAMETERS){
        sqlQuery = sql( "CREATE TABLE IF NOT EXISTS %1 ("
                        "param_id INTEGER PRIMARY KEY ON CONFLICT REPLACE,"
                        "name TEXT,"
                        "initial REAL,"
                        "minimum REAL,"
                        "maximum REAL,"
                        "library_id INTEGER"
                        ")").arg(TABLE_NAME_PARAMETERS);
    }


    if(!sqlQuery.isEmpty()){

    }

}

void ParameterStorage::saveToCache(const LibraryModel &library) {

    //! Если содержится в кэше, то обновляем
    if( cachedLibraries_.contains(library.id()) ){
        cachedLibraries_[library.id()] = library;
    }

    if(cachedLibraries_.size() <= CACHE_SIZE_PARAMETER_STORAGE){
        cachedLibraries_.insert( library.id(), library );
    }else{
        //! Удаляем первый элемент кэша
        cachedLibraries_.erase( cachedLibraries_.begin() );
        cachedLibraries_.insert( library.id(), library );
    }

}
