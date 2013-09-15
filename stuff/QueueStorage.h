#ifndef __ATM_TERMINAL__QUEUESTORAGE_H__
#define __ATM_TERMINAL__QUEUESTORAGE_H__

#include "common/ChequeEvents.h"
#include "common/Command.h"
#include "common/DbStorage.h"
#include "common/Payment.h"

/*!
 *  \page QueueStorage Доступ к платежам и командам
 *
 *  ATM поддерживает очередь команд и платежей, которая используется для
 *  обмена с сервером по специальному протоколу. Мы разделяем понятие платежей
 *  и команд в рамках АТМ, так как они несут разные типы информации, однако
 *  для сервера и платежи и команды являются сущностями одного порядка.
 *
 *  Некоторые нисходящие команды с сервера порождают постановку команд
 *  в очередь внутри ATM в результате реакции класса
 *  \ref atm::common::QueueManager на соответствующие события.
 *  Платежи и команды ставятся в очередь и обрабатываются
 *  в отдельном потоке классом \ref atm::common::QueueHandler.
 *
 *  Очередь команд хранится в постоянной памяти на диске в виде БД SQLite,
 *  и команды, еще не помещенные в архив по одобрению сервера, снова
 *  попадают в обработку при перезапуске ATM. Доступ к БД организует
 *  вспомогательный класс \ref atm::terminal::QueueStorage.
 *
 */

namespace atm {

namespace common {
namespace xml {
    class PaymentXML;
    class CommandXML;
}
}

namespace terminal {

enum CommandStates { COMMAND_NEW        = 0,
                     COMMAND_PROCESSING = 1,
                     COMMAND_CHEATED    = 2,
                     COMMAND_ARCHIVED   = 3};

extern const QString COMMAND_STATE_NEW;
extern const QString COMMAND_STATE_PROCESSING;
extern const QString COMMAND_STATE_CHEATED;
extern const QString COMMAND_STATE_ARCHIVED;

class QueueStorage;

struct TransactionCtx {
    QueueStorage * mStorage;
    bool mIsCommitted;
    TransactionCtx(QueueStorage * storage, bool isCommitted)
        : mStorage(storage), mIsCommitted(isCommitted) {}
};

namespace detail {
template <class T>
struct proxy
{
    T * resource_;
};
} // detail


/*!
 *  \brief Вспомогательный класс для организации транзакций в QueueStorage.
 *
 *  Типовая схема использования такова:
 *  \code
 *  QueueTransaction transaction = terminal::QueueStorage::instance()->transaction();
 *  // ... работа с платежами, например:
 *  QList<qint64> payments = transaction.getNewPayments();
 *  foreach(qint64 paymentId, payments)
 *      transaction.setPaymentState(paymentId, terminal::COMMAND_PROCESSING);
 *  // ... и т.д.
 *  if (!transaction.commit()) {
 *      ufo::logE(this) << tr("Could not commit transaction!");
 *      // Переключение терминала в крест
 *      // ...
 *  }
 *  \endcode
 *
 *  Транзакция блокирует любые операции с QueueStorage, кроме публичных методов
 *  QueueTransaction. Любые потоки, обращающиеся к методам QueueStorage, кроме потока,
 *  создавшего тразнакцию, будут ожидать ее завершения.
 *
 *  \note Вызов методов класса QueueStorage до вызова QueueTransaction::commit() или
 *  разрушения объекта QueueTransaction  в потоке, создавшем транзакцию, приведет
 *  к \em дэдлоку. То есть, в рамках транзакции следует пользоваться только методами
 *  QueueTransaction, в том числе внутри любых вызываемых функций. Повторный вызов
 *  QueueStorage::transaction из одного потока приведет к дэдлоку. Вызов
 *  QueueStorage::transaction из разных потоков является безопасным.
 *
 *  Объект QueueTransaction снимает блокировку в QueueStorage в двух случаях:
 *   - при вызове commit();
 *   - при вызове деструктора, если до этого не был вызван commit().
 *     Деструктор автоматически откатывает все сделанные в транзакции изменения (ROLLBACK).
 *  Копирование объекта QueueTransaction невозможно, так что даже если commit() не
 *  будет вызван, блокировка снимется при выходе из области видимости или при
 *  выходе из метода или при возникновении исключения.
 *
 */
class ATM_EXPORT QueueTransaction : public QObject {
    Q_OBJECT
    friend class QueueStorage;

public: // Реализация конструктора перемещения
    ~QueueTransaction() throw();
    QueueTransaction (detail::proxy<TransactionCtx> p) throw (); // The proxy move constructor
    QueueTransaction & operator = (detail::proxy<TransactionCtx> p) throw ();
    void swap (QueueTransaction &m) throw ();
    operator detail::proxy<TransactionCtx> () throw (); // A helper conversion function. Note that it is non-const
private:
    explicit QueueTransaction (TransactionCtx * ctx);
    QueueTransaction (QueueTransaction &m) throw (); // The "Move constructor" (note non-const parameter)
    QueueTransaction & operator = (QueueTransaction &m) throw (); // Move-assignment operator (note non-const parameter)

    TransactionCtx * mCtx;

public:
    bool commit();
    bool rollback();

    bool addPayment(common::Payment * payment, QByteArray paymentXml);
    bool setPaymentState(qint64 commandId, CommandStates state);
    bool setCommandState(qint64 commandId, CommandStates state);
    bool setPaymentId(qint64 commandId, qint64 newId, common::xml::PaymentXML *paymentXml);
    bool restoreCommand(qint64 commandId, common::Command::CommandType type);
    QList<qint64> getNewPayments();
    QList<qint64> getNewCommands();
    QList<qint64> getCommands(CommandStates state);
    QByteArray getCommandXml(qint64 commandId, common::Command::CommandType type);

    QString lastError();

    qint64 nextTerminalCommandId();

private:
    void unlock();
};


/*!
 *  \brief QueueStorage выступает хранилищем платежей и команд и имеет
 *  методы для создания и изменения состояния платежей и команд.
 *
 *  \note QueueStorage является синглтоном. Для каждого потока открывает собственное
 *  соединение с БД, т.к. Qt не поддерживает использование одного объекта
 *  QSqlDatabase в потоке, отличном от того, в котором он создан.
 *  Открытые методы класса \em являются потоково безопасными.
 *
 *  Поддерживаются два взаимоисключающих способа работы с QueueStorage:
 *   - через отдельные методы класса (addPayment(), restoreCommand() и т.д.);
 *   - через создание транзакции при помощи метода transaction().
 *  transaction() возвращает объект класса QueueTransaction, через
 *  который ведется дальнейшая работа с хранилищем. После выполнения
 *  всех необходимых изменений вызывающий код должен вызывать метод
 *  QueueTransaction::commit() или QueueTransaction::rollback() для
 *  завершения транзакции в базе.
 *  Если QueueTransaction::commit() или QueueTransaction::rollback()
 *  не будут вызваны, то при разрушении объекта QueueTransaction все
 *  сделанные изменения будут отменены в деструкторе (будет автоматически
 *  вызван rollback()).
 *
 *  \note При изменении кода полезно помнить, что методы с суффиксом
 *  \code Impl \endcode являются приватными и никогда не захватывают мьютекс.
 *  Открытые члены класса, в свою очередь, не имеют такого суффикса и
 *  в большинстве своем блокируют мьютекс на время выполнения.
 */
class ATM_EXPORT QueueStorage : public DbStorage {
    friend class QueueTransaction;
    Q_OBJECT

public:
    static QueueStorage * instance();
    QString dbName() const;

    qint64 nextTerminalCommandId();
    bool lastTerminalCommandId(qint64 commandId);

    bool addPayment(common::Payment * payment, QByteArray paymentXml);
    bool setPaymentState(qint64 commandId, CommandStates state);
    bool setPaymentId(qint64 commandId, qint64 newId, common::xml::PaymentXML *paymentXml);
    bool archivePayment(qint64 commandId);
    bool setChequeStatus(qint64 commandId, QString status);

    bool addCommand(common::Command * command, common::xml::CommandXML * commandXml);
    bool setCommandState(qint64 commandId, CommandStates state);
    bool setCommandId(qint64 commandId, qint64 newId);
    bool archiveCommand(qint64 commandId);

    bool restoreCommand(qint64 commandId, common::Command::CommandType type);
    /*! \brief Возвращает "новые" платежи. Интерфейс к getIdsImpl(). */
    QList<qint64> getNewPayments();
    /*! \brief Возвращает "новые" команды и event-ы. Интерфейс к getIdsImpl(). */
    QList<qint64> getNewCommands();

    QList<qint64> getProcessingCommands(common::Command::CommandType type);

    qint64 getPaymentsCount();
    struct ATM_EXPORT PaymentEntry {
        qint64 id;
        double sum;
        QString targetFid, targetValue;
        QDateTime dateTime;
        QString state;
//        common::ChequeStatus chequeStatus;
        QString chequeStatus;
        common::Payment payment;

        PaymentEntry();
        PaymentEntry(const PaymentEntry & other);
    };
    QVector<PaymentEntry> getPayments(qint64 from, qint64 count = -1);

    /*! \brief Возвращает XML команды, хранящийся в базе. Кодировка текста --- UTF-8.
     *  \note Возможные побочные эффекты: массив содержит текст в кодировке UTF-8,
     *  как и xml при сериализации. При использании метода нужно иметь это ввиду. */
    QByteArray getCommandXml(qint64 commandId, common::Command::CommandType type);

    QueueTransaction transaction();

    /*! \brief Переносит файл базы в архив с именем queue-yyyy-MM-dd-hh-mm-ss.db
     *  и открывает соединение с новым файлом. Возвращает признак успеха операции. */
    bool archiveStorage();

private:
    explicit QueueStorage(QObject * parent = 0);
    ~QueueStorage();

    // Неблокирующие реализации публичных методов класса.
    bool addPaymentImpl(common::Payment * payment, QByteArray paymentXml);
    bool setPaymentStateImpl(qint64 commandId, CommandStates state);
    bool setPaymentIdImpl(qint64 commandId, qint64 newId, QByteArray &paymentXml);
    bool setChequeStatusImpl(qint64 commandId, QString status);

    bool addCommandImpl(common::Command * command, common::xml::CommandXML * commandXml);
    bool setCommandStateImpl(qint64 commandId, CommandStates state);
    bool setCommandIdImpl(qint64 commandId, qint64 newId);

    bool restoreCommandImpl(qint64 commandId, common::Command::CommandType type);
    QByteArray getCommandXmlImpl(qint64 commandId, common::Command::CommandType type);

    // Вспомогательные методы.
    QString connectionName() const;

    bool needInit();
    bool init();
    qint64 lastTerminalCommandIdImpl();
    qint64 nextTerminalCommandIdImpl();

    enum TableType { IDS_TABLE, PAYMENTS_TABLE, COMMANDS_TABLE };
    bool createTableImpl(TableType type);

    /*! Возвращает команды в определенном состоянии. */
    QList<qint64> getIdsImpl(common::Command::CommandType type, CommandStates state);
    qint64 getPaymentsCountImpl();
    QVector<PaymentEntry> getPaymentsImpl(qint64 from, qint64 count);

    static QueueStorage * getInstance();
    static QString stateToString(CommandStates state);
    static QMutex mutex;
};

}
}

#endif // QUEUESTORAGE_H
