#ifndef DEVICESTORAGE_H
#define DEVICESTORAGE_H

#include <singleton.h>
#include "dbstorage.h"

namespace tsunami{
namespace db{
class DeviceModel;

class DeviceStorage : public DbStorage, public Singleton<DeviceStorage> {
enum DeviceTable{
    TABLE_DEVICES
};
public:
    DeviceStorage();
    ~DeviceStorage();
     QString dbName() const;
     DeviceModel* openDevice(int deviceId);
     bool saveDevice( DeviceModel* device );

     QMap<QString,int> listDevices( bool onlyEnabled = true );

protected:
    QString connectionName() const;
    int lastInsertId(const QString& table);
private:

    void testData();

    bool createTable(DeviceTable table);

    DeviceModel* openDeviceImpl(int deviceId);
    bool saveDeviceImpl( DeviceModel* device );

    static QString TABLE_NAME_DEVICES;
    static QString CONNECTION_NAME_DEVICES;
};

}
}

#endif // DEVICESTORAGE_H