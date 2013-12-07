#include "devicestorage.h"
#include "models/devicemodel.h"

namespace tsunami{
namespace db{

QString DeviceStorage::TABLE_NAME_DEVICES = QString("devices");
QString DeviceStorage::CONNECTION_NAME_DEVICES = QString("connection_devices");

DeviceStorage::DeviceStorage() : DbStorage(0){
    if(!tableExists(TABLE_NAME_DEVICES)){
        createTable(TABLE_DEVICES);
    }
}
DeviceStorage::~DeviceStorage(){

}

QString DeviceStorage::dbName() const{
    return DBASE_COMMON_NAME;
}

DeviceModel* DeviceStorage::openDevice(int deviceId){
    return openDeviceImpl(deviceId);
}
bool DeviceStorage::saveDevice(DeviceModel *device){
    return saveDeviceImpl(device);
}

bool DeviceStorage::removeDevice(int deviceId) {
    return removeDeviceImpl( deviceId );
}

QMap<QString, int> DeviceStorage::listDevices(bool onlyEnabled) {
    QMap<QString,int> list;
    QString sqlQuery;
    if(onlyEnabled){
        sqlQuery = sql( "SELECT id,name FROM %1 WHERE enable=:enable").arg(TABLE_NAME_DEVICES);
    }else{
        sqlQuery = sql( "SELECT id,name FROM %1").arg(TABLE_NAME_DEVICES);
    }

     QSqlQuery q( sqlQuery, db() );
     if(onlyEnabled){
         q.bindValue(":enable", true);
     }
     if(!q.exec()){
         setLastError( q.lastError().text() );
         return QMap<QString,int>();
     }

     QSqlRecord rec;
     while( q.next() ){
         rec = QSqlRecord(q.record());
         list.insert( ITEM("name").toString(), ITEM("id").toInt() );
     }

     return list;

}

QString DeviceStorage::connectionName() const {
    return CONNECTION_NAME_DEVICES;
}

int DeviceStorage::lastInsertId(const QString &table) {
    QString sqlQuery;
    if(table.compare(TABLE_NAME_DEVICES) == 0){
        sqlQuery = sql("SELECT MAX(id) FROM %1").arg(table);

        QSqlQuery q( sqlQuery, db() );
        if(q.exec() && q.next()){
            bool ok;
            int id = q.value(0).toInt(&ok);
            if(ok){
                return id;
            }
        }

    }

    return -1;
}

void DeviceStorage::testData() {
    DeviceModel* device = new DeviceModel();

    device->id(1);
    device->name("TEST BJT");
    device->type( DEVICE_NBJT );
    device->createAt( QDateTime::currentDateTime() );
    device->changeAt(QDateTime::currentDateTime() );
    device->enable( true );

    saveDevice( device );

}

bool DeviceStorage::createTable(DeviceStorage::DeviceTable table) {
    setLastError( QString() );
    QString sqlQuery("");

    if(table == TABLE_DEVICES){
        sqlQuery = sql( "CREATE TABLE IF NOT EXISTS %1("
                        "id INTEGER PRIMARY KEY ON CONFLICT REPLACE,"
                        "name TEXT,"
                        "type TEXT,"
                        "created_at NUMERIC,"
                        "changed_at NUMERIC,"
                        "enable NUMERIC"
                        ")")
                .arg( TABLE_NAME_DEVICES );
    }else{
        return false;
    }

    QSqlQuery q(sqlQuery,db());
    if( !q.exec() ){
        setLastError( q.lastError().text() );
        return false;
    }

#ifdef QT_DEBUG
    testData();
#endif

    return true;
}

DeviceModel *DeviceStorage::openDeviceImpl(int deviceId) {
    QString sqlQuery;
    sqlQuery = sql( "SELECT * FROM %1 WHERE id=:id").arg(TABLE_NAME_DEVICES);
    QSqlQuery q(sqlQuery,db());
    q.bindValue(":id",deviceId);

    if( !q.exec() || !q.next() ){
        setLastError( q.lastError().text() );
        Q_ASSERT(false);
        return NULL;
    }

    DeviceModel* device = new  DeviceModel();
    QSqlRecord rec(q.record() );

    device->id( ITEM("id").toInt() );
    device->name(ITEM("name").toString());
    device->type(ITEM("type").toString());
    device->createAt(ITEM("created_at").toDateTime());
    device->changeAt(ITEM("changed_at").toDateTime());
    device->enable(ITEM("enable").toBool());

    return device;

}

bool DeviceStorage::saveDeviceImpl(DeviceModel *device) {
    QString sqlQuery;
    int deviceId = device->id();

    if(!beginTransaction()){
        return false;
    }


    sqlQuery = sql( "INSERT OR REPLACE INTO %1(id,name,type,created_at,changed_at,enable) "
                    "VALUES(:id,:name,:type,:created_at,:changed_at,:enable)")
            .arg(TABLE_NAME_DEVICES);

    QSqlQuery q( sqlQuery, db() );

    if(device->id() == -1){
        deviceId = lastInsertId(TABLE_NAME_DEVICES)+1;
    }

    q.bindValue(":id",deviceId);
    q.bindValue(":name",device->name());
    q.bindValue(":type",device->typeJson());
    q.bindValue(":created_at",device->createAt());
    q.bindValue(":changed_at",device->changeAt());
    q.bindValue(":enable",device->enable());

    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }

    if(!endTransaction()){
        rollback();
        return false;
    }

    device->id( deviceId );

    return true;
}

bool DeviceStorage::removeDeviceImpl(int deviceId) {
    QString sqlQuery;

    if(!beginTransaction()){
        return false;
    }


    sqlQuery = sql( "DELETE FROM %1 WHERE id=:id")
            .arg(TABLE_NAME_DEVICES);

    QSqlQuery q( sqlQuery, db() );

    q.bindValue(":id",deviceId);

    if(!q.exec()){
        setLastError( q.lastError().text() );
        rollback();
        return false;
    }

    if(!endTransaction()){
        rollback();
        return false;
    }

    return true;

}




}
}
