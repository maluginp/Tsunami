#include "common/ChequeEvents.h"
#include "common/Command.h"
#include "common/Payment.h"
#include "common/xml/CommandXMLRecordImpl.h"
#include "common/xml/PaymentXMLRecordImpl.h"
#include "common/QueueStorage.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "ufo/uLog.h"
#include "utils.h"


namespace atm {
namespace terminal {

const QString QUEUE_TABLE_PREFIX       = "q_";
const QString QUEUE_PAYMENTS_TABLE     = "q_payments";
const QString QUEUE_COMMANDS_TABLE     = "q_commands";
const QString QUEUE_IDS_TABLE          = "q_ids";

extern ATM_EXPORT const QString COMMAND_STATE_NEW        = "new";
extern ATM_EXPORT const QString COMMAND_STATE_PROCESSING = "processing";
extern ATM_EXPORT const QString COMMAND_STATE_CHEATED    = "cheated";
extern ATM_EXPORT const QString COMMAND_STATE_ARCHIVED   = "archived";

QMutex QueueStorage::mutex;

QueueStorage::QueueStorage(QObject *parent) : DbStorage(parent) {
    ufo::logT(this) << tr("QueueStorage created");
    if (needInit())
        init();
}

QueueStorage::~QueueStorage()
{
}

QueueStorage*
QueueStorage::instance() {
    static QueueStorage * instance = 0;
    if (instance == 0) {
        QMutexLocker lock(&mutex);
        instance = getInstance();
    }
    return instance;
}

qint64
QueueStorage::nextTerminalCommandId() {
    QMutexLocker lock(&mutex);
    return nextTerminalCommandIdImpl();
}

bool QueueStorage::lastTerminalCommandId(qint64 commandId) {
    QMutexLocker lock(&mutex);
    setLastError(QString());
    QSqlQuery q(QString("INSERT OR IGNORE INTO '%1' (id) "
                        "SELECT max( :NEWVAL, ( SELECT max(id) FROM '%1' ) )").arg(QUEUE_IDS_TABLE), db());
    q.bindValue(":NEWVAL", commandId);
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logE(this) << tr("Could not set the last terminal command id: %1").arg(q.lastError().text());
        return false;
    }
    return true;
}

QueueStorage*
QueueStorage::getInstance() {
    ufo::AutoLogT(al, "atm::terminal::QueueStorage",  Q_FUNC_INFO);
    static QueueStorage instance;
    return &instance;
}

QString
QueueStorage::stateToString(CommandStates state) {
    switch (state) {
    case COMMAND_NEW:        return COMMAND_STATE_NEW;
    case COMMAND_PROCESSING: return COMMAND_STATE_PROCESSING;
    case COMMAND_CHEATED:    return COMMAND_STATE_CHEATED;
    case COMMAND_ARCHIVED:   return COMMAND_STATE_ARCHIVED;
    default: return QString();
    }
}

bool
QueueStorage::addPayment(common::Payment *payment, QByteArray paymentXml) {
    QMutexLocker lock(&mutex);
    return addPaymentImpl(payment, paymentXml);
}

bool
QueueStorage::setPaymentState(qint64 commandId, CommandStates state) {
    QMutexLocker lock(&mutex);
    return setPaymentStateImpl(commandId, state);
}

bool
QueueStorage::setPaymentId(qint64 commandId, qint64 newId, common::xml::PaymentXML *paymentXml) {
    QByteArray serialized = paymentXml->serialize();
    QMutexLocker lock(&mutex);
    return setPaymentIdImpl(commandId, newId, serialized);
}

bool
QueueStorage::archivePayment(qint64 commandId) {
    QMutexLocker lock(&mutex);
    return setPaymentStateImpl(commandId, COMMAND_ARCHIVED);
}

bool
QueueStorage::setChequeStatus(qint64 commandId, QString status) {
    QMutexLocker lock(&mutex);
    return setChequeStatusImpl(commandId, status);
}

bool
QueueStorage::addCommand(common::Command *command, common::xml::CommandXML *commandXml) {
    QMutexLocker lock(&mutex);
    return addCommandImpl(command, commandXml);
}

bool
QueueStorage::setCommandId(qint64 commandId, qint64 newId) {
    QMutexLocker lock(&mutex);
    return setCommandIdImpl(commandId, newId);
}

bool
QueueStorage::setCommandState(qint64 commandId, CommandStates state) {
    QMutexLocker lock(&mutex);
    return setCommandStateImpl(commandId, state);
}

bool
QueueStorage::archiveCommand(qint64 commandId) {
    QMutexLocker lock(&mutex);
    return setCommandStateImpl(commandId, COMMAND_ARCHIVED);
}

bool
QueueStorage::restoreCommand(qint64 commandId, common::Command::CommandType type) {
    QMutexLocker lock(&mutex);
    return restoreCommandImpl(commandId, type);
}

QList<qint64>
QueueStorage::getNewPayments(){
    QMutexLocker lock(&mutex);
    return getIdsImpl(common::Command::PAYMENT, COMMAND_NEW);
}

QList<qint64>
QueueStorage::getNewCommands() {
    QMutexLocker lock(&mutex);
    return getIdsImpl(common::Command::COMMAND, COMMAND_NEW);
}

QList<qint64>
QueueStorage::getProcessingCommands(common::Command::CommandType type) {
    QMutexLocker lock(&mutex);
    return getIdsImpl(type, COMMAND_PROCESSING);
}

qint64
QueueStorage::getPaymentsCount() {
    QMutexLocker lock(&mutex);
    return getPaymentsCountImpl();
}

QVector<QueueStorage::PaymentEntry>
QueueStorage::getPayments(qint64 from, qint64 count) {
    return getPaymentsImpl(from, count);
}

QByteArray
QueueStorage::getCommandXml(qint64 commandId, common::Command::CommandType type) {
    QMutexLocker lock(&mutex);
    return getCommandXmlImpl(commandId, type);
}

QueueTransaction
QueueStorage::transaction() {
    ufo::AutoLogT(al, this, "Replying to request to QueueStorage::transaction()");
    // Создаем QueueTransaction до того, как блокируем мьютекс.
    // Если мы внезапно вызовем исключение в new, то мьютекс
    // останется в неизменном состоянии.
    // Если не вызовем, то мьютекс разблокируется при вызове
    // QueueTransaction::commit, rollback или ~QueueTransaction.
    QueueTransaction ret(new TransactionCtx(this, false));
    mutex.lock();
    ufo::logT(this) << tr("Locked the mutex");
    if (!db().transaction()) {
        mutex.unlock();
        ret.mCtx->mIsCommitted = true;
        ret = QueueTransaction(new TransactionCtx(this, true));
    }
    return ret;
}

bool QueueStorage::archiveStorage() {
    QMutexLocker locker(&mutex);
    closeAll();

    QFile oldQueue(qApp->applicationDirPath() + QDir::separator() + dbName());
    QString archiveName;
    int lastDot = dbName().lastIndexOf('.');
    if (lastDot != -1) {
        archiveName = dbName().left(lastDot) + QDateTime::currentDateTime().toString("-yyyy-MM-dd-hh-mm-ss") +
                      dbName().right(dbName().length() - lastDot);
    } else {
        archiveName = dbName() + QDateTime::currentDateTime().toString("-yyyy-MM-dd-hh-mm-ss");
    }

    QDir appDir(qApp->applicationDirPath());
    QString archivePath = QString("queue%1archive").arg(QDir::separator());
    if (!appDir.mkpath(archivePath)) {
        ufo::logE(this) << tr("Cannot create a directory for a queue archive.");
        return false;
    } else if (!oldQueue.rename(qApp->applicationDirPath() + QDir::separator() + archivePath + QDir::separator() + archiveName)) {
        ufo::logE(this) << tr("Cannot move queue database %1 into %2").arg(oldQueue.fileName())
                           .arg(qApp->applicationDirPath() + QDir::separator() + archivePath + QDir::separator() + archiveName);
        return false;
    }

    return init();
}


bool
QueueStorage::addPaymentImpl(common::Payment *payment, QByteArray paymentXml) {
    setLastError(QString());
    QSqlQuery q(QString::fromLatin1("INSERT INTO %1 (id,sum,target_fid,target_value,datetime,state,cheque_status,xml) "
                                    "VALUES (:ID,:SUM,:TARGETFID,:TARGETVALUE,:DATETIME,:STATE,:CHEQUE_STATUS,:XML)")
                                    .arg(QUEUE_PAYMENTS_TABLE), db());
    q.bindValue(":ID",  payment->commandId());
    q.bindValue(":SUM", payment->sum());
    common::PaymentParam::ParamList params = payment->params();
    foreach(common::PaymentParam param, params) {
        if (param.isTarget()) {
            q.bindValue(":TARGETFID", param.name());
            q.bindValue(":TARGETVALUE", param.value());
            break;
        }
    }
    q.bindValue(":DATETIME",      payment->datetime());
    q.bindValue(":STATE",         stateToString(COMMAND_NEW));
    q.bindValue(":CHEQUE_STATUS", common::ChequeStatus::NO_CHEQUE);
    q.bindValue(":XML",           paymentXml);

    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Could not save payment into table %1: %2").arg(QUEUE_PAYMENTS_TABLE, q.lastError().text());
        return false;
    }
    return true;
}

bool
QueueStorage::setPaymentStateImpl(qint64 commandId, CommandStates state) {
    setLastError(QString());
    QSqlQuery q(QString::fromLatin1("UPDATE %1 SET state=:STATE WHERE id=:ID").arg(QUEUE_PAYMENTS_TABLE), db());
    q.bindValue(":STATE", stateToString(state));
    q.bindValue(":ID", commandId);
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Cannot change state of payment %1: %2").arg(commandId).arg(q.lastError().text());
        return false;
    }
    return true;
}

bool
QueueStorage::setPaymentIdImpl(qint64 commandId, qint64 newId, QByteArray &paymentXml) {
    setLastError(QString());
    QSqlQuery q(QString::fromLatin1("UPDATE %1 SET id=:NEWID, xml=:XML WHERE id=:ID").arg(QUEUE_PAYMENTS_TABLE), db());
    q.bindValue(":NEWID", newId);
    q.bindValue(":XML", paymentXml);
    q.bindValue(":ID", commandId);
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Cannot change id of payment %1: %2").arg(commandId).arg(q.lastError().text());
        return false;
    }
    return true;
}

bool
QueueStorage::setChequeStatusImpl(qint64 commandId, QString status) {
    setLastError(QString());
    QSqlQuery q(QString("UPDATE '%1' SET cheque_status=:STATUS WHERE id='%2'")
                .arg(QUEUE_PAYMENTS_TABLE).arg(commandId), db());
    q.bindValue(":STATUS", status);
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Cannot mark payment %1 as having its check printed: %2")
                           .arg(commandId).arg(q.lastError().text());
        return false;
    }
    return true;
}

bool
QueueStorage::addCommandImpl(common::Command *command, common::xml::CommandXML *commandXml) {
    setLastError(QString());
    QSqlQuery q(QString::fromLatin1("INSERT INTO %1 (id,datetime,type,state,xml) "
                                    "VALUES (:ID,:DATETIME,:TYPE,:STATE,:XML)").arg(QUEUE_COMMANDS_TABLE), db());
    q.bindValue(":ID",       command->id());
    q.bindValue(":DATETIME", command->datetime());
    q.bindValue(":TYPE",     command->commandType() == common::Command::EVENT ? QVariant::fromValue(command->typeId()) : QVariant());
    q.bindValue(":STATE",    stateToString(COMMAND_NEW));
    q.bindValue(":XML",      utils::serializeCommandToXML(commandXml));

    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Cannot save command into table %1: %2").arg(QUEUE_COMMANDS_TABLE, q.lastError().text());
        return false;
    }
    return true;
}

bool
QueueStorage::setCommandStateImpl(qint64 commandId, CommandStates state) {
    setLastError(QString());
    if (state == COMMAND_CHEATED) {
        ufo::logW(this) << tr("Logical error: a command cannot be in state %1").arg(stateToString(state));
        Q_ASSERT_X(state != COMMAND_CHEATED, "QueueStorage::setCommandStateImpl", "Logical error: a command cannot be in state COMMAND_CHEATED");
        return false;
    }

    QSqlQuery q(QString::fromLatin1("UPDATE %1 SET state=:STATE WHERE id=:ID").arg(QUEUE_COMMANDS_TABLE), db());
    q.bindValue(":STATE", stateToString(state));
    q.bindValue(":ID", commandId);
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Cannot change state of command %1: %2").arg(commandId).arg(q.lastError().text());
        return false;
    }
    return true;
}

bool
QueueStorage::setCommandIdImpl(qint64 commandId, qint64 newId) {
    setLastError(QString());
    QSqlQuery q(QString::fromLatin1("UPDATE %1 SET id=:NEWID WHERE id=:ID").arg(QUEUE_COMMANDS_TABLE), db());
    q.bindValue(":NEWID", newId);
    q.bindValue(":ID", commandId);
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Cannot change id of command %1: %2").arg(commandId).arg(q.lastError().text());
        return false;
    }
    return true;
}

bool
QueueStorage::restoreCommandImpl(qint64 commandId, common::Command::CommandType type) {
    if (type == common::Command::PAYMENT) return setPaymentStateImpl(commandId, COMMAND_NEW);
    if (type == common::Command::COMMAND ||
        type == common::Command::EVENT)   return setCommandStateImpl(commandId, COMMAND_NEW);
    return true;
}

QByteArray
QueueStorage::getCommandXmlImpl(qint64 commandId, common::Command::CommandType type) {
    QString table;
    if (type == common::Command::COMMAND || type == common::Command::EVENT)
        table = QUEUE_COMMANDS_TABLE;
    if (type == common::Command::PAYMENT)
        table = QUEUE_PAYMENTS_TABLE;
    QSqlQuery q(QString("SELECT xml FROM '%1' WHERE id=:ID").arg(table), db());
    q.bindValue(":ID", commandId);
    if (!q.exec() || !q.next()) {
        setLastError(q.lastError().text());
        ufo::logE(this) << tr("Cannot select command %1 from %2: %3").arg(commandId).arg(table, q.lastError().text());
        return QByteArray();
    }
    return q.value(0).toByteArray();
}

QString
QueueStorage::dbName() const {
    return "queue.db";
}

QString
QueueStorage::connectionName() const {
#if defined(Q_WS_WIN)
    return QString("queue_t%1").arg(QString::number(reinterpret_cast<quint64>(QThread::currentThreadId()), 16));
#elif defined(Q_WS_X11)
    return QString("queue_t%1").arg(QString::number(QThread::currentThreadId(), 16));
#else
    #error Not implemented for this platform.
#endif
}

bool
QueueStorage::needInit() {
    QString prefix  = QString(QUEUE_TABLE_PREFIX).replace(QChar('_'), QString("?_"));
    QSqlQuery q(QString("SELECT name FROM sqlite_master "
                        "WHERE type='table' AND name LIKE '%1%' ESCAPE '?' "
                        "ORDER BY name").arg(prefix), db());
    if (!q.exec()) {
        return false;
    }
    QStringList tables;
    while(q.next())
        tables << q.value(0).toString();
    return !tables.contains(QUEUE_COMMANDS_TABLE)
        || !tables.contains(QUEUE_PAYMENTS_TABLE)
        || !tables.contains(QUEUE_IDS_TABLE);
}

bool
QueueStorage::init() {
    setLastError(QString());
    QSqlDatabase & dbc = db();
    if (!dbc.transaction()) {
        setLastError(dbc.lastError().text());
        ufo::logE(this) << tr("Cannot begin transaction: %1").arg(dbc.lastError().text());
        return false;
    }
    if (   createTableImpl(IDS_TABLE)
        && createTableImpl(PAYMENTS_TABLE)
        && createTableImpl(COMMANDS_TABLE)
    ) {
        dbc.commit();
        return true;
    } else {
        dbc.rollback();
        return false;
    }
}

qint64 QueueStorage::lastTerminalCommandIdImpl() {
    setLastError(QString());
    qint64 ret = 0;
    QSqlQuery q(QString("SELECT MAX(id) FROM '%1'").arg(QUEUE_IDS_TABLE), db());
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logE(this) << tr("Could not select max id from %1: %2").arg(QUEUE_IDS_TABLE, q.lastError().text());
    } else if (q.next()) {
        ret = q.value(0).toLongLong();
    }
    return ret;
}

qint64 QueueStorage::nextTerminalCommandIdImpl() {
    setLastError(QString());
    QSqlQuery q(QString("INSERT INTO '%1' (id) VALUES (:ID)").arg(QUEUE_IDS_TABLE), db());
    qint64 id = lastTerminalCommandIdImpl() + 1;
    q.bindValue(":ID", id);
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logE(this) << tr("Could not reserve a command id: %1").arg(q.lastError().text());
    }
    return id;
}

bool
QueueStorage::createTableImpl(QueueStorage::TableType type) {
    setLastError(QString());
    switch (type) {
    case IDS_TABLE:
    {
        QString queryString = QString::fromLatin1("CREATE TABLE IF NOT EXISTS '%1' ("
                                                  "id INTEGER PRIMARY KEY)").arg(QUEUE_IDS_TABLE);
        ufo::logT(this) << tr("Query string: %1").arg(queryString);
        QSqlQuery q(queryString, db());
        if (!q.exec()) {
            setLastError(q.lastError().text());
            ufo::logE(this) << tr("Cannot create table %1: %2").arg(QUEUE_IDS_TABLE, q.lastError().text());
            return false;
        }
        return true;
    }
    case PAYMENTS_TABLE:
    {
        QString queryString = QString::fromLatin1("CREATE TABLE IF NOT EXISTS '%1' ("
                                                  "id            INTEGER  PRIMARY KEY"
                                                  "              REFERENCES %2 (id), "
                                                  "sum           REAL     NOT NULL, "
                                                  "target_fid    TEXT     NOT NULL, "
                                                  "target_value  TEXT     NOT NULL, "
                                                  "datetime      DATETIME NOT NULL, "
                                                  "state         TEXT     NOT NULL, "
                                                  "cheque_status TEXT     NOT NULL, "
                                                  "xml           TEXT     NOT NULL)")
                                                  .arg(QUEUE_PAYMENTS_TABLE, QUEUE_IDS_TABLE);
        ufo::logT(this) << tr("Query string: %1").arg(queryString);
        QSqlQuery q(queryString, db());
        if (!q.exec()) {
            setLastError(q.lastError().text());
            ufo::logE(this) << tr("Cannot create table %1: %2").arg(QUEUE_PAYMENTS_TABLE, q.lastError().text());
            return false;
        }
        return true;
    }
    case COMMANDS_TABLE:
    {
        QString queryString = QString::fromLatin1("CREATE TABLE IF NOT EXISTS '%1' ("
                                                  "id       INTEGER  PRIMARY KEY"
                                                  "         REFERENCES %2 (id), "
                                                  "datetime DATETIME NOT NULL, "
                                                  "type     INTEGER, "          // Только для event-ов.
                                                  "state    TEXT     NOT NULL, "
                                                  "xml      TEXT     NOT NULL)")
                                                  .arg(QUEUE_COMMANDS_TABLE, QUEUE_IDS_TABLE);
        ufo::logT(this) << tr("Query string: %1").arg(queryString);
        QSqlQuery q(queryString, db());
        if (!q.exec()) {
            setLastError(q.lastError().text());
            ufo::logE(this) << tr("Cannot create table %1: %2").arg(QUEUE_COMMANDS_TABLE, q.lastError().text());
            return false;
        }
        return true;
    }
    default:
        return false; // unreachable, but compiler is happy
    }
}

QList<qint64>
QueueStorage::getIdsImpl(common::Command::CommandType type, CommandStates state) {
    setLastError(QString());
    QList<qint64> ret;
    QString query("SELECT id FROM '%1' WHERE state=:STATE");
    QString table;
    if (type == common::Command::COMMAND) table = QUEUE_COMMANDS_TABLE;
    if (type == common::Command::PAYMENT) table = QUEUE_PAYMENTS_TABLE;

    QSqlQuery q(query.arg(table), db());
    q.bindValue(":STATE", stateToString(state));
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Cannot select '%1' commands from %2: %3").arg(stateToString(state), table, q.lastError().text());
    }
    while(q.next()) {
        bool ok;
        qint64 id = q.value(0).toLongLong(&ok);
        if (ok) ret << id;
        else ufo::logW(this) << tr("Cannot convert value '%1' in '%2' that should be of type INTEGER PRIMARY KEY")
                                   .arg(q.value(0).toString(), table);
    }
    return ret;
}

qint64
QueueStorage::getPaymentsCountImpl() {
    setLastError(QString());
    QSqlQuery q(QString("SELECT COUNT(*) FROM [%1]").arg(QUEUE_TABLE_PREFIX + "payments"), db());
    if (!q.exec() || !q.next()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Cannot select count of payments in the database: %1").arg(q.lastError().text());
        return -1;
    }

    return q.value(0).toInt();
}

QVector<QueueStorage::PaymentEntry>
QueueStorage::getPaymentsImpl(qint64 from, qint64 count) {
    setLastError(QString());
    //                            0      1             2               3           4        5                6      7
    QSqlQuery q(QString("SELECT [id], [sum], [target_fid], [target_value], [datetime], [state], [cheque_status], [xml]\n"
                        "FROM '%1' ORDER BY id DESC LIMIT :LIMIT OFFSET :OFFSET").arg(QUEUE_TABLE_PREFIX + "payments"), db());
    q.bindValue(":LIMIT", count);
    q.bindValue(":OFFSET", from);
    if (!q.exec()) {
        setLastError(q.lastError().text());
        ufo::logW(this) << tr("Cannot select payments from the database: %1").arg(q.lastError().text());
        return QVector<PaymentEntry>();
    }

    QVector<PaymentEntry> ret;
    ret.reserve(count);
    while(q.next()) {
        PaymentEntry entry;
        // @todo обработка ошибок
        entry.id = q.value(0).toLongLong();
        entry.sum = q.value(1).toDouble();
        entry.targetFid = q.value(2).toString();
        entry.targetValue = q.value(3).toString();
        entry.dateTime = q.value(4).toDateTime();
        entry.state = q.value(5).toString();
        entry.chequeStatus = q.value(6).toString();

        QByteArray paymentXml = q.value(7).toByteArray();
        common::Payment * payment = 0;
        if (!paymentXml.isEmpty()) {
            QXmlStreamReader xmlReader(paymentXml);
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement() && xmlReader.name().compare("payment", Qt::CaseInsensitive) == 0) {
                    payment = common::xml::PaymentXML::deserialize(xmlReader);
                    entry.payment = *payment;
                    delete payment;
                    break;
                }
            }
            // @todo checkHash(xmlData, *payment);
        }
        ret.append(entry);
    }
    return ret;
}


///////////////// QueueTransaction /////////////////


QueueTransaction::QueueTransaction(TransactionCtx *ctx) : mCtx(ctx) {
}

QueueTransaction::~QueueTransaction() throw() {
    if (mCtx) {
        ufo::AutoLogT(al, this, "QueueTransaction::~QueueTransaction");
        if (!mCtx->mIsCommitted)
            rollback();
        delete mCtx;
    }
}

QueueTransaction::QueueTransaction(detail::proxy<TransactionCtx> p) throw()
    : mCtx(p.resource_) {
    // Just copying resource pointer is sufficient. No need to NULL it like in the move constructor.
}

QueueTransaction &
QueueTransaction::operator =(detail::proxy<TransactionCtx> p) throw() {
    // copy and swap idiom. Must release the original resource in the destructor.
    QueueTransaction temp (p);
    temp.swap(*this);
    return *this;
}

void
QueueTransaction::swap(QueueTransaction &m) throw() {
    std::swap(this->mCtx, m.mCtx);
}

QueueTransaction::operator detail::proxy<TransactionCtx>() throw() { // A helper conversion function. Note that it is non-const
    detail::proxy<TransactionCtx> p;
    p.resource_ = this->mCtx;
    this->mCtx = 0;     // Resource moved to the temporary proxy object.
    return p;
}

QueueTransaction::QueueTransaction(QueueTransaction &m) throw() // The "Move constructor" (note non-const parameter)
    : QObject(), mCtx(m.mCtx)
{
    m.mCtx = 0;
}

QueueTransaction &
QueueTransaction::operator =(QueueTransaction &m) throw() // Move-assignment operator (note non-const parameter)
{
    QueueTransaction temp(m);
    temp.swap(*this);
    return *this;
}

bool
QueueTransaction::commit() {
    ufo::AutoLogT(al, this, "QueueTransaction::commit");
    if (!mCtx->mIsCommitted) {
        bool ret = mCtx->mStorage->db().commit();
        mCtx->mIsCommitted = true;
        unlock();
        return ret;
    } else {
        ufo::logE(this) << tr("A try to make a commit after commit or rollback have already been made");
        return true;
    }
}

bool
QueueTransaction::rollback() {
    ufo::AutoLogT(al, this, "QueueTransaction::rollback");
    if (!mCtx->mIsCommitted) {
        bool ret = mCtx->mStorage->db().rollback();
        mCtx->mIsCommitted = true;
        unlock();
        return ret;
    } else {
        ufo::logE(this) << tr("A try to make a rollback after commit or rollback have already been made");
        return true;
    }
}

void
QueueTransaction::unlock() {
    mCtx->mStorage->mutex.unlock();
}

bool
QueueTransaction::addPayment(common::Payment *payment, QByteArray paymentXml) {
    if (!mCtx->mIsCommitted) return mCtx->mStorage->addPaymentImpl(payment, paymentXml);
    else return false;
}

bool
QueueTransaction::setPaymentState(qint64 commandId, CommandStates state) {
    if (!mCtx->mIsCommitted) return mCtx->mStorage->setPaymentStateImpl(commandId, state);
    else return false;
}

bool
QueueTransaction::setCommandState(qint64 commandId, CommandStates state) {
    if (!mCtx->mIsCommitted) return mCtx->mStorage->setCommandStateImpl(commandId, state);
    else return false;
}

bool
QueueTransaction::setPaymentId(qint64 commandId, qint64 newId, common::xml::PaymentXML *paymentXml) {
    if (!mCtx->mIsCommitted) {
        QByteArray serialized = paymentXml->serialize();
        return mCtx->mStorage->setPaymentIdImpl(commandId, newId, serialized);
    } else
        return false;
}

bool
QueueTransaction::restoreCommand(qint64 commandId, common::Command::CommandType type) {
    if (!mCtx->mIsCommitted) return mCtx->mStorage->restoreCommandImpl(commandId, type);
    else return false;
}

QList<qint64>
QueueTransaction::getNewPayments() {
    if (!mCtx->mIsCommitted) return mCtx->mStorage->getIdsImpl(common::Command::PAYMENT, COMMAND_NEW);
    else return QList<qint64>();
}

QList<qint64>
QueueTransaction::getNewCommands() {
    if (!mCtx->mIsCommitted) return mCtx->mStorage->getIdsImpl(common::Command::COMMAND, COMMAND_NEW);
    else return QList<qint64>();
}

QList<qint64>
QueueTransaction::getCommands(CommandStates state) {
    if (!mCtx->mIsCommitted) return mCtx->mStorage->getIdsImpl(common::Command::COMMAND, state);
    else return QList<qint64>();
}

QByteArray
QueueTransaction::getCommandXml(qint64 commandId, common::Command::CommandType type) {
    if (!mCtx->mIsCommitted)
        return mCtx->mStorage->getCommandXmlImpl(commandId, type);
    else
        return QByteArray();
}

QString
QueueTransaction::lastError() {
    return mCtx->mStorage->lastError();
}

qint64 QueueTransaction::nextTerminalCommandId() {
    return mCtx->mStorage->nextTerminalCommandIdImpl();
}

QueueStorage::PaymentEntry::PaymentEntry()
    : payment(0)
{
}

QueueStorage::PaymentEntry::PaymentEntry(const QueueStorage::PaymentEntry &other)
    : id(other.id)
    , sum(other.sum)
    , targetFid(other.targetFid)
    , targetValue(other.targetValue)
    , dateTime(other.dateTime)
    , state(other.state)
    , chequeStatus(other.chequeStatus)
    , payment(other.payment)
{
}


}
}
