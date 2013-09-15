#ifndef DBSTORAGE_H
#define DBSTORAGE_H

#include <QObject>
#include <QtSql>

const QString dbMain = QString("base.db");

class DbStorage : public QObject {
    Q_OBJECT
public:

    bool isOpen();
    QString lastError();
    virtual QString dbName() = 0;

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

private:
    QSqlDatabase allocateDb();

    QHash<Qt::HANDLE, QSqlDatabase> databases_;
    QThreadStorage<QString*> lastErrors_;
};

#endif // DBSTORAGE_H
