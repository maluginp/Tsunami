#include "settingstorage.h"

namespace tsunami{
namespace db{


QString SettingStorage::TABLE_NAME_SETTINGS = QString("settings");
QString SettingStorage::CONNECTION_NAME_SETTING = QString("connection_settings");

SettingStorage::SettingStorage() : DbStorage(0) {
    if(!tableExists(TABLE_NAME_SETTINGS)){
        createTable( TABLE_SETTINGS );
    }
}

SettingStorage::~SettingStorage() {

}

QString SettingStorage::dbName() const {
    return DBASE_COMMON_NAME;
}

QVariant SettingStorage::value(const QString &key, const QVariant &defaultValue) {
    return valueImpl( key,defaultValue );
}

bool SettingStorage::saveValue(const QString &key, const QVariant &value) {
    return saveValueImpl( key,value );
}

QString SettingStorage::connectionName() const {
    return CONNECTION_NAME_SETTING;
}

int SettingStorage::lastInsertId(const QString &table) {
    QString sqlQuery;
    if(table.compare(TABLE_NAME_SETTINGS) == 0){
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

void SettingStorage::testData() {
    saveValue( "spice/simulator", "ngspice");
    saveValue( "optimize/max_iteration", 100);
    saveValue( "spice/ngspice/enable", true );
    saveValue( "optimize/tolerance/function", 1e-14);

//#ifdef Q_MAC_OS
    saveValue( "spice/ngspice/path", "/usr/local/bin/ngspice");
    saveValue( "optimize/method", "hookejeeves" );
//    saveValue( "optimize/hookejeeves/max_iteration", 100);
//    saveValue( "optimize/hookejeeves/tol" )
}

bool SettingStorage::saveValueImpl(const QString &key, const QVariant &value) {
    setLastError( QString() );
    QString sqlQuery("");

    sqlQuery = sql("INSERT OR REPLACE INTO %1(key,value) VALUES(:key,:value)")
            .arg( TABLE_NAME_SETTINGS );

    QSqlQuery q(sqlQuery,db());

    q.bindValue( ":key", key );
    q.bindValue( ":value", value );

    if(!q.exec()){
        setLastError( q.lastError().text() );
        return false;
    }

    return true;
}

QVariant SettingStorage::valueImpl(const QString &key, const QVariant &defaultValue) {
//    setLastError( QString() );
    QString sqlQuery("");

    sqlQuery = sql("SELECT value FROM %1 WHERE key=:key")
            .arg( TABLE_NAME_SETTINGS );

    QSqlQuery q(sqlQuery,db());
    q.bindValue(":key",key);

    if(!q.exec() || !q.next()){
//        setLastError( q.lastError().text() );
        return defaultValue;
    }


    return q.value(0);

}

bool SettingStorage::createTable(const SettingStorage::SettingTable &table){
    setLastError( QString() );
    QString sqlQuery("");

    if(table == TABLE_SETTINGS){
        sqlQuery = sql( "CREATE TABLE IF NOT EXISTS %1("
                        "key TEXT PRIMARY KEY ON CONFLICT REPLACE,"
                        "value TEXT)")
                .arg( TABLE_NAME_SETTINGS );
    }else{
        return false;
    }

    QSqlQuery q(sqlQuery,db());
    if( !q.exec() ){
        setLastError( q.lastError().text() );
        return false;
    }

#ifdef QT_DEBUG
    testData();
#endif
    return true;
}

}
}
