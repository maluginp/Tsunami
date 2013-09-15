#include "common/SettingsStorage.h"
#include "ufo/uLog.h"
#include <QtSql/QSqlQuery>

namespace atm {
namespace terminal {

const QString SettingsStorage::SETTINGS_DB_NAME = "settings.db";
const QString SettingsStorage::SETTINGS_TABLE_PREFIX = "s_";

SettingsStorage::SettingsStorage(QObject *parent) : DbStorage(parent)
{
}

SettingsStorage::~SettingsStorage()
{
}

QVariant SettingsStorage::value(const QString &propName, const QVariant &defValue) {
    QVariant value = selectValue(propName);
    if (value.isValid()) return value;
    else                 return defValue;
}

bool SettingsStorage::contains(const QString &propName) {
    return selectValue(propName).isValid();
}

bool SettingsStorage::saveValue(const QString &propName, const QVariant &value) {
    if (!db().isOpen()) {
        setLastError(db().lastError().text());
        ufo::logW(this) << tr("Database '%1' is not open").arg(dbName());
        return false;
    }

    QStringList sectionKey = propName.split("/");
    if (sectionKey.size() != 2) {
        return false;
    }
    QString section = sectionKey.at(0);
    QString key     = sectionKey.at(1);

    QString fullName = SETTINGS_TABLE_PREFIX + section;
    if (!(tableExists(fullName) || createSection(section))) {
        return false;
    }

    setLastError(QString());
    QSqlQuery q(db());
    q.prepare(QString::fromLatin1("INSERT OR REPLACE INTO %1 (key,value) VALUES (:KEY,:VAL)").arg(fullName));
    q.bindValue(":KEY", key);
    q.bindValue(":VAL", value);
    bool ret = true;
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Error while saving setting '%1': %2").arg(key, q.lastError().text());
        ret = false;
    }
    return ret;
}

QStringList SettingsStorage::sections() {
    setLastError(QString());
    if (!db().isOpen()) {
        setLastError(db().lastError().text());
        ufo::logW(this) << tr("Database '%1' is not open").arg(dbName());
        return QStringList();
    }

    QString prefix  = QString(SETTINGS_TABLE_PREFIX).replace(QChar('_'), QString("?_"));
    QSqlQuery q(QString::fromLatin1("SELECT name FROM sqlite_master WHERE type='table' AND name LIKE '%1%' ESCAPE '?'").arg(prefix), db());
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Error selecting names of tables (sections) from DB '%1': %2").arg(dbName(), q.lastError().text());
        return QStringList();
    }

    QStringList ret;
    while(q.next()) {
        ret << q.value(0).toString().remove(
                            QRegExp(QString::fromLatin1("^%1").arg(SETTINGS_TABLE_PREFIX)));
    }
    return ret;
}

QVariantMap SettingsStorage::section(const QString &section) {
    setLastError(QString());
    if (!db().isOpen()) {
        setLastError(db().lastError().text());
        ufo::logW(this) << tr("Database '%1' is not open").arg(dbName());
        return QVariantMap();
    }

    QString fullName = SETTINGS_TABLE_PREFIX + section;
    QSqlQuery q(QString::fromLatin1("SELECT key, value FROM '%1'").arg(fullName), db());
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << QString::fromLatin1("Error selecting values from section (table) '%1': %2").arg(fullName, q.lastError().text());
        return QVariantMap();
    }

    QVariantMap ret;
    while(q.next()) {
        ret.insert(QString::fromLatin1("%1/%2").arg(section,q.value(0).toString()), q.value(1));
    }
    return ret;
}

bool SettingsStorage::beginTransaction() {
    setLastError(QString());
    QSqlDatabase & dbc = db();
    if (!dbc.isOpen()) {
        setLastError("Database is not open");
        return false;
    }
    if (!dbc.transaction()) {
        setLastError(dbc.lastError().text());
        ufo::logW(this) << tr("Could not begin transaction: %1").arg(dbc.lastError().text());
        return false;
    }
    return true;
}

bool SettingsStorage::endTransaction() {
    setLastError(QString());
    QSqlDatabase & dbc = db();
    if (!dbc.isOpen()) {
        setLastError("Database is not open");
        return false;
    }
    if (!dbc.commit()) {
        setLastError(dbc.lastError().text());
        ufo::logW(this) << tr("Could not end transaction: %1").arg(dbc.lastError().text());
        return false;
    }
    return true;
}

bool SettingsStorage::createSection(const QString &section) {
    setLastError(QString());
    QString fullName = SETTINGS_TABLE_PREFIX + section;
    QSqlQuery q(QString::fromLatin1("CREATE TABLE IF NOT EXISTS %1 ('key' TEXT PRIMARY KEY, value TEXT)").arg(fullName), db());
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Error creating table '%1': %2").arg(fullName, q.lastError().text());
        return false;
    }
    return true;
}

QVariant SettingsStorage::selectValue(const QString &propName) {
    setLastError(QString());
    if (!db().isOpen()) {
        setLastError("Database is not open");
        ufo::logW(this) << tr("Database '%1' is not open").arg(dbName());
        return QVariant();
    }

    QStringList sectionKey = propName.split("/");
    if (sectionKey.size() != 2)
        return QVariant();
    QString section = sectionKey.at(0);
    QString key     = sectionKey.at(1);

    QString fullName = SETTINGS_TABLE_PREFIX + section;

    if (!tableExists(fullName))
        return QVariant();

    QSqlQuery q(QString::fromLatin1("SELECT value FROM %1 WHERE key='%2'").arg(fullName,key), db());
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Cannot select value of '%1' in table '%2' : %3").arg(key, fullName, q.lastError().text());
        return QVariant();
    }
    if (!q.next())
        return QVariant();
    else {
        return q.value(0);
    }
}

QString SettingsStorage::dbName() const {
    return SETTINGS_DB_NAME;
}

QString SettingsStorage::connectionName() const {
#if defined(Q_WS_WIN)
    return QString("settings_t%1").arg(QString::number(reinterpret_cast<quint64>(QThread::currentThreadId()), 16));
#elif defined(Q_WS_X11)
    return QString("settings_t%1").arg(QString::number(QThread::currentThreadId(), 16));
#else
    #error Not implemented for this platform.
#endif
}

}
}
