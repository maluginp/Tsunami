#include "parameterstorage.h"
#include "../models/librarymodel.h"

QString ParameterStorage::TABLE_NAME_PARAMETERS = QString("parameters");
QString ParameterStorage::TABLE_NAME_LIBRARIES  = QString("libraries");
QString ParameterStorage::CONNECTION_NAME_PARAMETER = QString("parameter_connection");


ParameterStorage::ParameterStorage() : DbStorage(0) {
    if( !tableExists(TABLE_NAME_LIBRARIES)){
        createTable(TABLE_LIBRARIES);
    }

    if( !tableExists( TABLE_NAME_PARAMETERS ) ){
        createTable( TABLE_PARAMETERS );
    }
}

ParameterStorage::~ParameterStorage() {

}

QString ParameterStorage::dbName() const {
    return DbStorage::DBASE_COMMON_NAME;

}

const LibraryModel &ParameterStorage::library() const{
    return currentLibrary_;
}

bool ParameterStorage::saveLibrary() {
    bool res = saveLibraryImpl( currentLibrary_ );
    return res;
}

bool ParameterStorage::saveLibrary(const LibraryModel &library) {
    bool res = saveLibraryImpl( library );
    return res;
}

bool ParameterStorage::addParameterToLibrary(const ParameterModel &parameter) {
    return addParameterToLibraryImpl( parameter );
}

LibraryModel ParameterStorage::openLibrary(const int &libraryId){
    return openLibraryImpl(libraryId);
}

void ParameterStorage::setCurrentLibrary(const int &libraryId) {
    if(currentLibrary_.id() == libraryId){
        return;
    }

    if( cachedLibraries_.contains(libraryId) ){
        currentLibrary_ = cachedLibraries_[libraryId];
    }else{
        currentLibrary_ = openLibrary( libraryId );
    }
}

QString ParameterStorage::connectionName() const {
    return CONNECTION_NAME_PARAMETER;
}

bool ParameterStorage::saveLibraryImpl(const LibraryModel &library) {

    QString sqlQuery;
    int lastInsertId;
    currentLibrary_ = library;

    //! Start commit
    if(!beginTransaction()){
        return false;
    }

    sqlQuery = sql("INSERT OR REPLACE INTO %1( library_id, name, project_id, user_id, create_at, change_at, enable) "
                   "       VALUES (:library_id,:name,:project_id,:user_id,:create_at,:change_at,:enable)")
            .arg(TABLE_NAME_LIBRARIES);


    QSqlQuery q( sqlQuery, db() );

    q.bindValue(":name",       currentLibrary_.name());
    q.bindValue(":project_id", currentLibrary_.projectId() );
    q.bindValue(":user_id",    currentLibrary_.userId() );
    q.bindValue(":create_at",  currentLibrary_.createAt());
    q.bindValue(":change_at",  currentLibrary_.changeAt());
    q.bindValue(":enable",     currentLibrary_.enable());

    //! TODO: strong check
    if(currentLibrary_.id() == -1){
        lastInsertId = q.lastInsertId().toInt();
        currentLibrary_.setId(lastInsertId);
    }

    q.bindValue(":library_id", currentLibrary_.id() );

    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }



    sqlQuery = sql("INSERT OR REPLACE INTO %1(param_id, name, initial, minimum, maximum, library_id)"
                   "        VALUES(:param_id,:name,:initial,:minimum,:maximum,:library_id")
            .arg(TABLE_NAME_PARAMETERS);

    foreach(ParameterModel parameter,currentLibrary_.parameters()){
        q = QSqlQuery( sqlQuery, db() );

        if(parameter.id() != -1 ){
            lastInsertId = -1;
            parameter.setId(  lastInsertId  );

            currentLibrary_.setParameter( parameter.name(), parameter );
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


    saveCache( currentLibrary_ );
    return true;
}

LibraryModel ParameterStorage::openLibraryImpl(const int &libraryId) {
    setLastError(QString());

    if(cachedLibraries_.contains(libraryId)){
        return cachedLibraries_[libraryId];
    }

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
    library.setName(      q.value(rec.indexOf("name")).toString()        );
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
        rec = QSqlRecord(q.record());
        ParameterModel parameter;
        parameter.setId(       q.value( rec.indexOf("param_id")).toInt()   )
                .setName(      q.value( rec.indexOf("name") ).toString()   )
                .setInitial(   q.value( rec.indexOf("initial")).toDouble() )
                .setMinimum(   q.value( rec.indexOf("minimum")).toDouble() )
                .setMaximum(   q.value( rec.indexOf("maximum")).toDouble() )
                .setLibraryId( q.value( rec.indexOf("libraryId") ).toInt() );

        if(parameter.id() != -1){
            library.parameters_.append( parameter );
        }
    }



    currentLibrary_ = library;
    saveCache( library );

    return library;
}

bool ParameterStorage::addParameterToLibraryImpl(const ParameterModel &parameter) {
    if(currentLibrary_.parameterExists( parameter.name() )){
        return false;
    }

    currentLibrary_.parameters_.append( parameter );
    saveLibraryImpl( currentLibrary_ );
    saveCache( currentLibrary_);
    return true;
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
    }else{
        return false;
    }


    QSqlQuery q(sqlQuery,db());
    if(!q.exec()){
        setLastError(q.lastError().text());
        return false;
    }

    return true;
}

void ParameterStorage::saveCache(const LibraryModel &library) const {

    //! Если содержится в кэше, то обновляем
    if( cachedLibraries_.contains(library.id()) ){
        cachedLibraries_[library.id()] = library;
        return;
    }

    if(cachedLibraries_.size() > CACHE_SIZE_PARAMETER_STORAGE){
        //! Удаляем первый элемент кэша
        cachedLibraries_.erase( cachedLibraries_.begin() );
    }
    cachedLibraries_.insert( library.id(), library );

}
