#ifndef SETTINGSTORAGE_H
#define SETTINGSTORAGE_H
#include <QtCore>
#include <singleton.h>
#include "DbStorage.h"
namespace tsunami{
namespace db{

class TSUNAMI_EXPORT SettingStorage : public DbStorage, public Singleton<SettingStorage> {
    enum SettingTable{
        TABLE_SETTINGS
    };

public:
    SettingStorage();
    ~SettingStorage();

    QString dbName() const;

    QVariant value( const QString& key, const QVariant& defaultValue = QVariant() );
    bool saveValue( const QString& key, const QVariant& value );
protected:
    QString connectionName() const;
    int lastInsertId(const QString& table);
private:
    void testData();
    bool saveValueImpl( const QString& key, const QVariant& value );
    QVariant valueImpl( const QString& key, const QVariant& defaultValue = QVariant() );

    bool createTable(const SettingTable& table);

    static QString TABLE_NAME_SETTINGS;
    static QString CONNECTION_NAME_SETTING;

};

}
}


#endif // SETTINGSTORAGE_H
