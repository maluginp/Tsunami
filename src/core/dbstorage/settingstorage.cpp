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

    return true;
}

}
}
