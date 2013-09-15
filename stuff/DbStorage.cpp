#include "common/DbStorage.h"

#include <QStringList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "ufo/uLog.h"

namespace atm {
namespace terminal {

DbStorage::DbStorage(QObject *parent): QObject(parent) {
}

DbStorage::~DbStorage() {
    closeAll();
}

bool DbStorage::isOpen() {
    return db().isOpen();
}

QString DbStorage::lastError() {
    return *mLastErrors.localData();
}

bool DbStorage::tableExists(const QString &table) {
    setLastError(QString());
    QSqlQuery q(QString::fromLatin1("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'").arg(table), db());
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logT(this) << tr("Error occured while checking for existence of table '%1': %2").arg(table, q.lastError().text());
        return false;
    }
    return q.next();
}

QSqlDatabase & DbStorage::db() {
    // Метод не используется самостоятельно, о блокировках заботится вызывающий.
    Qt::HANDLE tid = QThread::currentThreadId();
    if (!mDbs.contains(tid)) { // В этом потоке уже создавался экземпляр БД?
        mDbs[tid] = allocateDb();
    }
    return mDbs[tid];
}

void DbStorage::closeAll() {
    foreach(Qt::HANDLE h, mDbs.keys()) {
        QString connectionName = mDbs[h].connectionName();
        mDbs[h].close();
        mDbs.remove(h);
        QSqlDatabase::removeDatabase(connectionName);
    }
}

void DbStorage::setLastError(QString msg) {
    mLastErrors.setLocalData(new QString(msg));
}

QSqlDatabase DbStorage::allocateDb() {
    // Метод не используется самостоятельно, о блокировках заботится вызывающий.
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName());
    db.setDatabaseName(dbName());
    if (!db.open()) {
        setLastError(db.lastError().text());
        ufo::logE(this) << tr("Cannot open database '%1': %2").arg(dbName(), db.lastError().text());
    }
    QSqlQuery q("PRAGMA foreign_keys = ON", db);
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logE(this) << tr("Could not set PRAGMA foreign_keys to ON. The queue storage will not work correctly.");
    }
    return db;
}


}
}
