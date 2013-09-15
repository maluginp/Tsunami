#ifndef __ATM_TERMINAL__DBSTORAGE_H__
#define __ATM_TERMINAL__DBSTORAGE_H__

#include "atmmacros.h"
#include <QtCore>
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QThreadStorage>

namespace atm {
namespace terminal {

/*!
 *  \brief Класс DbStorage предоставляет базовый доступ к БД (SQLite).
 *  Является предком \ref atm::terminal::SettingsStorage и
 *  \ref atm::terminal::QueueStorage.
 *
 *  Предоставляет доступ к БД через метод QSqlDatabase & db().
 *  Для каждого потока открывает собственный экзепляр соединения
 *  (QSqlDatabase), и db() возвращает вызывающему нужный экземпляр,
 *  основываясь на QThread::currentThreadId(). Остальные методы
 *  обращаются к базе данных через метод db(), тем самым всегда
 *  получая соединение, соответствующее текущему потоку.
 *
 *  \attention Методы класса \em не являются потоково безопасными.
 */

class ATM_EXPORT DbStorage : public QObject
{
    Q_OBJECT
public:
    /*! Возвращает true, если соединение с БД открыто.
     *  Кандидат на перенос в SettingsStorage, а то и на удаление,
     *  ибо кажется избыточным. */
    bool isOpen();
    /*! Возвращает последнюю ошибку БД. */
    QString lastError();
    /*! Возвращает имя БД. */
    virtual QString dbName() const = 0;

protected:
    explicit DbStorage(QObject *parent = 0);
    /*! Закрывает базы данных (mDbs). */
    virtual ~DbStorage();

    /*! Возвращает \с true, если таблица \a table существует в базе. */
    bool tableExists(const QString & table);

    /*! Возвращает имя соединения с учетом threadId. */
    virtual QString connectionName() const = 0;

    /*! Возвращает экземпляр БД, созданный в вызывающем потоке. */
    QSqlDatabase & db();

    /*! Закрывает соединения с БД для всех потоков и освобождает файл. */
    void closeAll();

    void setLastError(QString msg);

private:
    /*! Создает и возвращает соединение с БД для текущего потока */
    QSqlDatabase allocateDb();
    /*! Хранилище соединений */
    QHash<Qt::HANDLE, QSqlDatabase> mDbs;
    QThreadStorage<QString*> mLastErrors; // Храним указатели, а не сами строки, потому что под линукс и Qt 4.7.0 поддерживаются только указатели:
                                          // "Note that due to compiler limitations, QThreadStorage can only store pointers."
};

}
}

#endif
