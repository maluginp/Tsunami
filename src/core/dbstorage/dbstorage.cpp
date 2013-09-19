#include "dbstorage.h"
QString DbStorage::DBASE_COMMON_NAME = QString("base.db");


DbStorage::DbStorage(QObject *parent) :
    QObject(parent) {
}

DbStorage::~DbStorage() {

}

bool DbStorage::isOpen() {
    return db().isOpen();
}

QString DbStorage::lastError() {
    return *lastErrors_.localData();
}

QString DbStorage::sql(const char *sql) {
    return QString::fromLatin1( sql );
}


bool DbStorage::tableExists(const QString &table) {
    setLastError(QString());
    QSqlQuery q(QString::fromLatin1("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'").arg(table), db());
    if (!q.exec()) {
        setLastError(q.lastError().text());
        return false;
    }
    return q.next();
}

QSqlDatabase &DbStorage::db() {
    Qt::HANDLE threadId = QThread::currentThreadId();
    if(!databases_.contains(threadId)){
        databases_[threadId] = allocateDb();
    }

    return databases_[threadId];
}

void DbStorage::closeAll() {
    foreach( Qt::HANDLE threadId, databases_.keys() ){
        QString connectionName = databases_[threadId].connectionName();
        databases_[threadId].close();
        databases_.remove(threadId);
        QSqlDatabase::removeDatabase(connectionName);
    }
}

void DbStorage::setLastError(const QString &msg) {
    lastErrors_.setLocalData( new QString(msg) );
}

bool DbStorage::beginTransaction() {
    setLastError( QString() );
    QSqlDatabase& dbc = db();
    if(!dbc.isOpen()){
        setLastError( QString("Database is not open") );
        return false;
    }
    if(!dbc.transaction()){
        setLastError( dbc.lastError().text() );
        return false;
    }

    return true;

}

bool DbStorage::rollback(){
    setLastError( QString() );
    QSqlDatabase& dbc = db();
    if(!dbc.isOpen()){
        setLastError( QString("Database is not open") );
        return false;
    }
    if(!dbc.rollback()){
        setLastError( dbc.lastError().text() );
        return false;
    }

    return true;
}

bool DbStorage::endTransaction() {
    setLastError( QString() );
    QSqlDatabase& dbc = db();
    if(!dbc.isOpen()){
        setLastError( QString("Database is not open") );
        return false;
    }
    if(!dbc.commit()){
        setLastError( dbc.lastError().text() );
        return false;
    }
    return true;
}

bool DbStorage::dropTable(const QString &table) {
    setLastError(QString());
    QString sqlQuery = sql( "DROP TABLE %1" ).arg(table);

    QSqlQuery q(sqlQuery,db());
    if(!q.exec()){
        setLastError( q.lastError().text() );
        return false;
    }

    return true;
}

QSqlDatabase DbStorage::allocateDb() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName());
    db.setDatabaseName(dbName());
    if (!db.open()) {
        setLastError(db.lastError().text());
    }

    QSqlQuery q("PRAGMA foreign_keys = ON", db);

    if (!q.exec()) {
        setLastError(q.lastError().text());
    }

    return db;
}




