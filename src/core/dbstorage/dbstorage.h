#ifndef DBSTORAGE_H
#define DBSTORAGE_H

#include <QtCore>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

class DbStorage : public QObject {
    Q_OBJECT
public:

    bool isOpen();
    QString lastError();
    virtual QString dbName() const = 0;

    QString sql(const char* sql);

protected:
    explicit DbStorage(QObject *parent = 0);
    virtual ~DbStorage();

    bool tableExists(const QString& table);
    virtual QString connectionName() const = 0;
    QSqlDatabase& db();
    void closeAll();
    void setLastError(const QString& msg);

    bool createTable(const QString& table, const QStringList& columns);

    QMutex dbMutex_;
    static QString DBASE_COMMON_NAME;
private:
    QSqlDatabase allocateDb();

    QHash<Qt::HANDLE, QSqlDatabase> databases_;
    QThreadStorage<QString*> lastErrors_;
};

#endif // DBSTORAGE_H
