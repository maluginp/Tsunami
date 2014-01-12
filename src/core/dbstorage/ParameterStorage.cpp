#include "ParameterStorage.h"

#include "Log.h"
namespace tsunami{
namespace db{

QString ParameterStorage::TABLE_NAME_PARAMETERS = QString("parameters");
QString ParameterStorage::TABLE_NAME_LIBRARIES  = QString("libraries");
QString ParameterStorage::TABLE_NAME_TEMPLATE_LIBRARIES = QString("lib_templates");
QString ParameterStorage::CONNECTION_NAME_PARAMETER = QString("parameter_connection");

#define SYNC_TEMPLATE_DIRECTORY true

ParameterStorage::ParameterStorage() : DbStorage(0) {
    if( !tableExists(TABLE_NAME_LIBRARIES)){
        createTable(TABLE_LIBRARIES);
    }

    if( !tableExists( TABLE_NAME_PARAMETERS ) ){
        createTable( TABLE_PARAMETERS );
    }

    if( !tableExists(TABLE_NAME_TEMPLATE_LIBRARIES)){
        createTable( TABLE_TEMPLATE_LIBRARIES );
    }

#ifdef SYNC_TEMPLATE_DIRECTORY
    syncTemplateDirectory();
#endif

}

ParameterStorage::~ParameterStorage() {

}

QString ParameterStorage::dbName() const {
    return DbStorage::DBASE_COMMON_NAME;

}

bool ParameterStorage::saveLibrary(LibraryModel *library) {
    bool res = saveLibraryImpl( library );
    return res;
}

bool ParameterStorage::saveTemplateLibrary(LibraryTemplateModel *templateLibrary) {
    return saveTemplateLibraryImpl( templateLibrary );
}


LibraryModel* ParameterStorage::openLibrary(int libraryId){
    return openLibraryImpl(libraryId);
}

int ParameterStorage::numberLibraries(int deviceId) {
    int nLibraries = 0;
    QString sqlQuery;
    sqlQuery = sql("SELECT id FROM %1 WHERE device_id=:device_id").arg(TABLE_NAME_LIBRARIES);

    QSqlQuery q(sqlQuery,db());
    q.bindValue(":device_id", deviceId);

    if(!q.exec()){
        setLastError( q.lastError().text() );
        return 0;
    }


    while(q.next()){
        nLibraries++;
    }

    return nLibraries;

}

LibraryTemplateModel *ParameterStorage::openTemplateLibraryImpl(const QString &templateName) {

    QString sqlQuery;

    sqlQuery = sql( "SELECT * FROM %1 WHERE name=:name")
                  .arg(TABLE_NAME_TEMPLATE_LIBRARIES);

    QSqlQuery q(sqlQuery,db());
    q.bindValue(":name",templateName);

    if(q.exec() && q.next()){
        QSqlRecord rec(q.record());
        LibraryTemplateModel* templateLibrary = new LibraryTemplateModel();

        templateLibrary->name( ITEM("name").toString() );
        templateLibrary->devices( ITEM("devices").toString() );
        templateLibrary->parameters(ITEM("parameters").toString() );

        return templateLibrary;
    }


    return NULL;
}

QMap<int,QString>  ParameterStorage::listLibraries(int deviceId){
    return listLibrariesImpl(deviceId);
}

QList<LibraryModel *> ParameterStorage::getLibrariesByDeviceId(int deviceId) {
    return getLibrariesByDeviceIdImpl(deviceId);
}

QList<LibraryModel *> ParameterStorage::getTemplateLibrariesByDeviceType(DeviceType type) {
    setLastError(QString());

    QString sqlQuery;
    // Load Library

    sqlQuery = sql("SELECT name FROM %1").arg(TABLE_NAME_TEMPLATE_LIBRARIES);

    QSqlQuery q(sqlQuery,db());

    if(!q.exec()){
        setLastError( q.lastError().text() );
        return QList<LibraryModel*>();
    }

    QList<LibraryModel*> libraries;

    while(q.next()){
        QSqlRecord rec = QSqlRecord(q.record());

        LibraryTemplateModel* templateLibrary =  openTemplateLibraryImpl( ITEM("name").toString());
        if(templateLibrary->satisfyDevice(type)){
            libraries.append( templateLibrary->convertToLibraryModel() );
        }
        delete templateLibrary;
    }

    return libraries;
}

LibraryModel *ParameterStorage::openTemplateLibrary(const QString &templateName) {
    LibraryTemplateModel* templateLibrary = openTemplateLibraryImpl(templateName);

    if(templateLibrary != NULL){
        LibraryModel* library = templateLibrary->convertToLibraryModel();
        delete templateLibrary;

        return library;
    }

    return NULL;
}

bool ParameterStorage::removeLibrary(int libraryId)
{
    return removeLibraryImpl(libraryId);
}

bool ParameterStorage::exists(int deviceId,const QString &name) {
    QString sqlQuery;
    sqlQuery = sql( "SELECT id FROM %1 WHERE name=:name AND device_id=:device_id")
               .arg(TABLE_NAME_LIBRARIES);

     QSqlQuery q( sqlQuery, db() );
     q.bindValue(":name", name);
     q.bindValue(":device_id",deviceId);

     if(!q.exec()){
         setLastError( q.lastError().text() );
         return true;
     }

     int count = 0;
     while(q.next()){
         count++;
     }

     if(count == 0){
         return false;
     }

     return true;
}

QString ParameterStorage::connectionName() const {
    return CONNECTION_NAME_PARAMETER;
}

int ParameterStorage::lastInsertId(const QString &table) {
    QString sqlQuery;
    if(table.compare(TABLE_NAME_PARAMETERS) == 0 ||
            table.compare(TABLE_NAME_LIBRARIES) == 0){
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

void ParameterStorage::testData() {

    LibraryModel* library = new LibraryModel();
    library->id(1);
    library->deviceId(1);
    library->name("LibraryTest");
    library->createAt(  QDateTime::currentDateTime() );
    library->changeAt( QDateTime::currentDateTime() );
    library->enable(true);

    // Parameters
    for(int i=1; i < 11; ++i) {

        ParameterModel parameter;
        parameter.id( i );
        parameter.libraryId( library->id() );
        parameter.name( QString("PARAM_%1").arg(i) );
        parameter.initial( 1.0+i*1.5 );
        parameter.fitted( parameter.initial() );
        parameter.minimum( 0.0 );
        parameter.maximum( 10.0 + i*2.5 );
        parameter.fixed(false);
        parameter.enable(true);

        library->addParameter( parameter );

    }

    if(!saveLibrary(library)){
        // Error
    }

}

void ParameterStorage::syncParameters(LibraryModel *library) {
    if(library->id() == -1){
        return;
    }
    QList<int> parameterIndex;
    QString sqlQuery;
    sqlQuery = sql( "SELECT id FROM %1 WHERE library_id=:library_id").arg(TABLE_NAME_PARAMETERS);
    QSqlQuery q( sqlQuery, db() );
    q.bindValue(":library_id", library->id());
    if(!q.exec()){
        setLastError( q.lastError().text() );
        return;
    }

    while( q.next() ) {
        parameterIndex.append( q.value(0).toInt() );
    }

    sqlQuery = sql("DELETE FROM %1 WHERE id=:id").arg(TABLE_NAME_PARAMETERS);
    foreach(int index, parameterIndex){
        bool exists = false;
        foreach(ParameterModel parameter, library->parameters()){
            if(parameter.id() == index){
                exists = true;
                break;
            }
        }
        if(!exists){
            q = QSqlQuery(sqlQuery,db());
            q.bindValue(":id",index);
            if(!q.exec()){
                setLastError(q.lastError().text());
            }
        }
    }

    return;
}

QMap<int, QString> ParameterStorage::listLibrariesImpl(int deviceId) {
    QMap<int, QString> items;
    QString sqlQuery;
    sqlQuery = sql("SELECT * FROM %1 WHERE device_id=:device_id").arg(TABLE_NAME_LIBRARIES);

    QSqlQuery q(sqlQuery,db());
    q.bindValue(":device_id", deviceId);

    if(!q.exec()){
        setLastError( q.lastError().text() );
        return QMap<int, QString>();
    }


    while(q.next()){
        QSqlRecord rec(q.record());
        items.insert( ITEM("id").toInt(), ITEM("name").toString() );
    }

    return items;
}

bool ParameterStorage::saveLibraryImpl(LibraryModel *library) {

    QString sqlQuery;
    int libraryId = library->id();

    //! Start commit
    if(!beginTransaction()){
        return false;
    }


    sqlQuery = sql( "INSERT OR REPLACE INTO %1(id,device_id,name,created_at,changed_at,enable) "
                    "VALUES(:id,:device_id,:name,:created_at,:changed_at,:enable)")
            .arg(TABLE_NAME_LIBRARIES);



    QSqlQuery q( sqlQuery, db() );
    bool needSync = true;
    if(libraryId == -1){
        libraryId = lastInsertId(TABLE_NAME_LIBRARIES) + 1;
        needSync = false;
    }

    q.bindValue(":id",          libraryId);
    q.bindValue(":device_id",   library->deviceId());
    q.bindValue(":name",        library->name());
    q.bindValue(":created_at",  library->createAt());
    q.bindValue(":changed_at",  library->changeAt());
    q.bindValue(":enable",      library->enable());

    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }


    if(needSync){
        syncParameters( library );
    }

    sqlQuery = sql("INSERT OR REPLACE INTO "
                   "%1(id,library_id,name,initial,fitted,minimum,maximum,fixed,enable) "
                   "VALUES "
                   "(:id,:library_id,:name,:initial,:fitted,:minimum,:maximum,:fixed,:enable)")
            .arg(TABLE_NAME_PARAMETERS);

    foreach(ParameterModel parameter,library->parameters()){
        if(parameter.name().isEmpty()){
            continue;
        }

        q = QSqlQuery( sqlQuery, db() );

        parameter.libraryId( libraryId );
        if(parameter.id() == -1 || !needSync){
            int parameterId = lastInsertId(TABLE_NAME_PARAMETERS)+1;
            parameter.id(  parameterId  );
        }

        library->parameter(parameter.name()) = parameter;

        q.bindValue(":id",         parameter.id() );
        q.bindValue(":library_id", parameter.libraryId());
        q.bindValue(":name",       parameter.name());
        q.bindValue(":initial",    parameter.initial());
        q.bindValue(":fitted",     parameter.fitted());
        q.bindValue(":minimum",    parameter.minimum());
        q.bindValue(":maximum",    parameter.maximum());
        q.bindValue(":fixed",      parameter.fixed());
        q.bindValue(":enable",     parameter.enable());

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


    library->id( libraryId );

    return true;
}

LibraryModel* ParameterStorage::openLibraryImpl(int libraryId) {
    setLastError(QString());

    QString sqlQuery;
    // Load Library
    LibraryModel* library = new LibraryModel();
    sqlQuery = sql("SELECT * FROM %1 WHERE id=:id").arg(TABLE_NAME_LIBRARIES);

    QSqlQuery q(sqlQuery,db());
    q.bindValue(":id",libraryId);

    if(!q.exec() || !q.next()){
        setLastError( q.lastError().text() );
        return NULL;
    }

    QSqlRecord rec(q.record());


    library->id(       ITEM("id").toInt() );
    library->deviceId( ITEM("device_id").toInt() );
    library->name(     ITEM("name").toString() );
    library->createAt( ITEM("created_at").toDateTime());
    library->changeAt( ITEM("changed_at").toDateTime() );
    library->enable(   ITEM("enable").toBool() );

    if(library->id() == -1){
        return NULL;
    }


    sqlQuery =sql("SELECT * FROM %1 WHERE library_id=:library_id").arg(TABLE_NAME_PARAMETERS);

    q = QSqlQuery(sqlQuery,db());
    q.bindValue( ":library_id", library->id() );

    if(!q.exec()){
        setLastError( q.lastError().text() );
        return NULL;
    }

    while( q.next() ){
        rec = QSqlRecord(q.record());
        ParameterModel parameter;

        parameter.id(        ITEM("id").toInt() );
        parameter.libraryId( ITEM("library_id").toInt());
        parameter.name(      ITEM("name").toString());
        parameter.initial(   ITEM("initial").toDouble());
        parameter.fitted(    ITEM("fitted").toDouble());
        parameter.minimum(   ITEM("minimum").toDouble());
        parameter.maximum(   ITEM("maximum").toDouble());
        parameter.fixed(     ITEM("fixed").toBool());
        parameter.enable(    ITEM("enable").toBool());

        if(parameter.id() != -1){
            library->addParameter( parameter );
        }
    }

    return library;
}

QList<LibraryModel *> ParameterStorage::getLibrariesByDeviceIdImpl(int deviceId){
    setLastError(QString());

    QString sqlQuery;
    // Load Library

    sqlQuery = sql("SELECT id FROM %1 WHERE device_id=:device_id").arg(TABLE_NAME_LIBRARIES);

    QSqlQuery q(sqlQuery,db());
    q.bindValue(":device_id",deviceId);

    if(!q.exec()){
        setLastError( q.lastError().text() );
        return QList<LibraryModel*>();
    }

    QList<LibraryModel*> libraries;
    LibraryModel* library;

    while(q.next()){
        QSqlRecord rec = QSqlRecord(q.record());
        library =  openLibrary( ITEM("id").toInt() );

        if(library != NULL){
            libraries.append( library );
        }
    }

    return libraries;

}

bool ParameterStorage::removeLibraryImpl(int libraryId){
    if(libraryId == -1){
        return false;
    }

    //! Start commit
    if(!beginTransaction()){
        return false;
    }

    QString sqlQuery;

    sqlQuery = sql("DELETE FROM %1 WHERE library_id=:library_id").arg(TABLE_NAME_PARAMETERS);

    QSqlQuery q = QSqlQuery(sqlQuery,db());
    q.bindValue(":library_id", libraryId);
    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }

    sqlQuery = sql("DELETE FROM %1 WHERE id=:library_id").arg(TABLE_NAME_LIBRARIES);

    q = QSqlQuery(sqlQuery,db());
    q.bindValue(":library_id", libraryId);
    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }

    if(!endTransaction()){
        rollback();
        return false;
    }


    return true;
}

bool ParameterStorage::saveTemplateLibraryImpl(LibraryTemplateModel *templateLibrary) {

    QString sqlQuery = sql("INSERT OR REPLACE INTO %1(name,devices,parameters) "
                           "VALUES(:name,:devices,:parameters)")
                          .arg(TABLE_NAME_TEMPLATE_LIBRARIES);


    if(!beginTransaction()){
        return false;
    }

    QSqlQuery q(sqlQuery,db());

    q.bindValue(":name",templateLibrary->name());
    q.bindValue(":devices",templateLibrary->devicesJson());
    q.bindValue(":parameters",templateLibrary->parametersJson());

    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }

    if(!endTransaction()){
        rollback();
        return false;
    }


    return true;

}

bool ParameterStorage::createTable(const ParameterStorage::ParameterTable &table) {
    QString sqlQuery;
    if(table == ParameterStorage::TABLE_LIBRARIES){
        sqlQuery = sql("CREATE TABLE IF NOT EXISTS %1 ("
                       "id INTEGER PRIMARY KEY ON CONFLICT REPLACE,"
                       "device_id INTEGER,"
                       "name TEXT,"
                       "created_at NUMERIC,"
                       "changed_at NUMERIC,"
                       "enable NUMERIC"
                       ")").arg(TABLE_NAME_LIBRARIES);

    }else if(table == ParameterStorage::TABLE_PARAMETERS){
        sqlQuery = sql("CREATE TABLE IF NOT EXISTS %1 ("
                       "id INTEGER PRIMARY KEY ON CONFLICT REPLACE,"
                       "library_id INTEGER,"
                       "name TEXT,"
                       "initial REAL,"
                       "fitted REAL,"
                       "minimum REAL,"
                       "maximum REAL,"
                       "fixed NUMERIC,"
                       "enable NUMERIC"
                       ")").arg(TABLE_NAME_PARAMETERS);

    }else if(table == ParameterStorage::TABLE_TEMPLATE_LIBRARIES){
        sqlQuery = sql("CREATE TABLE IF NOT EXISTS %1 ("
                       "name TEXT PRIMARY KEY ON CONFLICT REPLACE,"
                       "devices TEXT,"
                       "parameters TEXT)").arg(TABLE_NAME_TEMPLATE_LIBRARIES);

    }  else {
        return false;
    }


    QSqlQuery q(sqlQuery,db());
    if(!q.exec()){
        setLastError( q.lastError().text() );
        return false;
    }

//#ifdef QT_DEBUG
    if(tableExists(TABLE_NAME_LIBRARIES) && tableExists(TABLE_NAME_PARAMETERS)){
        testData();
    }
//#endif

    return true;

}

void ParameterStorage::syncTemplateDirectory() {
    log::logTrace() << "Syncing template directory";
    QList<LibraryTemplateModel*> templates = LibraryTemplateModel::parseDirectory();

    foreach( LibraryTemplateModel* templateLibrary, templates ){
        if(saveTemplateLibrary( templateLibrary )){
            log::logDebug() << QString("Template '%1' saved")
                               .arg(templateLibrary->name());
        }
    }

    return;
}


}
}

