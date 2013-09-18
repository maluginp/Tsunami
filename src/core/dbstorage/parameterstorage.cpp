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
    if( cachedLibraries_.contains(libraryId) ){
        lastLibrary_ = cachedLibraries_[libraryId];
    }

    lastLibrary_ = openLibrary( libraryId );

}

QString ParameterStorage::connectionName() const {
    return CONNECTION_NAME_PARAMETER;
}

bool ParameterStorage::saveLibraryImpl(const LibraryModel &library) {

    QString sqlQuery;
    bool isNew = false;
    if(library.id() == -1 ){
        isNew = true;
    }

    lastLibrary_ = library;

    //! Start commit
    if(!beginTransaction()){
        return false;
    }

    if(isNew){
        sqlQuery = sql("INSERT INTO %1( name, project_id, user_id, create_at, change_at, enable) "
                       "       VALUES (:name,:project_id,:user_id,:create_at,:change_at,:enable)")
                .arg(TABLE_NAME_LIBRARIES);
    }else{
        sqlQuery = sql("UPDATE %1 SET name=:name,"
                       "project_id=:project_id,"
                       "user_id=:user_id,"
                       "create_at=:create_at,"
                       "change_at=:change_at,"
                       "enable=:enable "
                       "WHERE library_id=:library_id")
                .arg(TABLE_NAME_LIBRARIES);
    }

    QSqlQuery q( sqlQuery, db() );

    q.bindValue(":name",       library.name());
    q.bindValue(":project_id", library.projectId() );
    q.bindValue(":user_id",    library.userId() );
    q.bindValue(":create_at",  library.createAt());
    q.bindValue(":change_at",  library.changeAt());
    q.bindValue(":enable",     library.enable());

    if(!isNew){ q.bindValue(":library_id",library.id()); }

    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }

    int lastInsertId;
    if(isNew){
        lastInsertId = q.lastInsertId();
    }else{
        lastInsertId = library.id();
    }


    sqlQuery = sql("INSERT OR REPLACE INTO %1( name, initial, minimum, maximum, library_id)"
                   "        VALUES(:name,:initial,:minimum,:maximum,:library_id")
            .arg(TABLE_NAME_PARAMETERS);

    foreach(ParameterModel parameter,library.parameters()){
        q = QSqlQuery( sqlQuery, db() );

        q.bindValue(":name",parameter.name());
        q.bindValue(":initial",parameter.initial());
        q.bindValue(":minimum",parameter.initial());
        q.bindValue(":maximum",parameter.maximum());
        q.bindValue(":library_id",lastInsertId);

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

    lastLibrary_.updateLibraryId( lastInsertId );

    saveToCache( lastLibrary_ );
    return true;
}

LibraryModel ParameterStorage::openLibraryImpl(const int &libraryId) {
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

    library.id() = q.value( rec.indexOf( "library_id" ) );
    library.name() = q.value( rec.indexOf("name") );
    library.projectId() = q.value( rec.indexOf("project_id") );
    library.userId()    = q.value( rec.indexOf("user_id") );
    library.createAt()  = q.value( rec.indexOf("create_at") ).toDateTime();
    library.changeAt()  = q.value( rec.indexOf("change_at") ).toDateTime();
    library.enable()    = q.value( rec.indexOf("enable") ).toBool();

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

        parameter.id()      = q.value( rec.indexOf("param_id")).toInt();
        parameter.name()    = q.value( rec.indexOf("name") ).toString();
        parameter.initial() = q.value( rec.indexOf("initial")).toDouble();
        parameter.minimum() = q.value( rec.indexOf("minimum")).toDouble();
        parameter.maximum() = q.value( rec.indexOf("maximum") ).toDouble();

        parameter.libraryId()  = q.value( rec.indexOf("libraryId") ).toInt();

        if(parameter.id() != -1){
            library.parameters_.append( parameter );
        }
    }



    lastLibrary_ = library;
    saveToCache( library );

    return library;
}

void ParameterStorage::addParameterToLibraryImpl(const ParameterModel &parameter) {
    if(!lastLibrary_.parameterExists( parameter.name() )){
        lastLibrary_.parameters_.append( parameter );
        saveLibraryImpl( lastLibrary_ );

        saveToCache( lastLibrary_);
    }
}

void ParameterStorage::createTable(const ParameterStorage::ParameterTable &table) {
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
