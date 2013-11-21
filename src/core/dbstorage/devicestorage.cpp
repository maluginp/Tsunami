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

QString DeviceStorage::connectionName() const {
    return CONNECTION_NAME_DEVICES;
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
    Q_ASSERT(false);
    return NULL;

}

bool DeviceStorage::saveDeviceImpl(DeviceModel *device) {
    Q_ASSERT(false);
    return NULL;
}




}
}
