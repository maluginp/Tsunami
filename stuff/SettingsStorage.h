#ifndef __ATM_TERMINAL__SETTINGSSTORAGE_H__
#define __ATM_TERMINAL__SETTINGSSTORAGE_H__

#include "common/DbStorage.h"

namespace atm {
namespace terminal {

/*!
 *  \brief Класс SettingsStorage предоставляет доступ к настройкам
 *  приложения через локальную БД (SQLite).
 *
 *  \attention Методы класса \em не являются потоково безопасными.
 *  Класс используется в \ref atm::terminal::Settings, который имеет
 *  мьютекс на всех операциях создания и использования SettingsStorage.
 *
 *  Настройки хранятся в SQLite по секциям в виде таблиц со столбцами
 *  "key" и "value". Например, если мы имеем настройку
 *  "printer/configured" = false, то в БД ей будет соответствовать таблица
 *  с именем s_printer и двумя столбцами --- [key] TEXT PRIMARY KEY и value TEXT.
 */

class ATM_EXPORT SettingsStorage : public DbStorage {
    Q_OBJECT

public:
    explicit SettingsStorage(QObject * parent = 0);
    ~SettingsStorage();

    /*! \~russian Возвращает значение свойства \a propName.
     *  \param[in] propName --- имя свойства в виде "section/variable".
     *  \param[in] defValue --- возвращается по умолчанию, если невозможно найти значение. */
    QVariant value(const QString & propName, const QVariant & defValue = QString(""));

    /*! \~russian Возвращает \c true, если \a propName существует в базе.
     *  \param[in] propName --- имя свойства в виде "section/variable". */
    bool contains (const QString & propName);

    /*! \~russian Сохраняет значение свойства \a propName в базе.
     *  \param[in] propName --- имя свойства в виде "section/variable".
     *  \param[in] value --- значение свойства.
     *  \returns \c true, если операция завершилась успешно. */
    bool saveValue(const QString & propName, const QVariant & _value);

    /*! \~russian Возвращает имена секций, имеющихся в базе.
     *  \returns \c QStringList(), если ни одна секция не найдена. */
    QStringList sections();

    /*! \~russian Возвращает имя и значение каждого свойства
     *  из секции \a section.
     *  \returns Каждая пара выглядит как ("section/variable", value),
     *  где value --- \c QVariant, содержащий значение
     *  свойства. Для пустых значений параметра (NULL или '') всегда
     *  выполняется value.isValid() == true. */
    QVariantMap section(const QString & section);

    /*! Начинает транзакцию. Полезно для ускорения больших вставок.
     *  \return Возвращает true, если транзакция успешно начата. */
    bool beginTransaction();
    /*! \return Возвращает true, если транзакция успешно закончена. */
    bool endTransaction();

private:
    /*! \~russian Создает таблицу с именем \a table. */
    bool createSection(const QString & section);

    /*! \~russian Делает SELECT свойства \a propName.
     *  \return Возвращает значение свойства, если оно существует в базе,
     *  или QVariant() --- в противном случае. Если значение в базе
     *  существует, и оно равно NULL или '', то у возвращаемого
     *  значения будет выполняться isValid() == true,
     *  а isNull() вернет true или false соответственно. */
    QVariant selectValue(const QString & propName);

    static const QString SETTINGS_DB_NAME;
    static const QString SETTINGS_TABLE_PREFIX;

    QString dbName() const;
    QString connectionName() const;
};

}
}

#endif // __ATM_TERMINAL__SETTINGSSTORAGE_H__
