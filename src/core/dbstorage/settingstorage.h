#ifndef SETTINGSTORAGE_H
#define SETTINGSTORAGE_H
#include <QtCore>
#include <singleton.h>
#include "dbstorage.h"

class SettingStorage : public DbStorage, public Singleton<SettingStorage> {
public:
    SettingStorage();

    QString dbName() const;

    QVariant value( const QString& key, const QVariant& defaultValue = QVariant() );
    bool saveValue( const QString& key, const QVariant& value );
protected:
    QString connectionName() const;
private:
    bool saveValueImpl( const QString& key, const QVariant& value );

};

#endif // SETTINGSTORAGE_H
