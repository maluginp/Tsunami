#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "defines.h"
#include <map>

namespace tsunami{
namespace spice{

class Terminal;
class Device;
class SpiceModel;

typedef QMap<int,Device*> DeviceMap;
typedef QList<SpiceModel*> ModelList;

class TSUNAMI_EXPORT Circuit {
public:
    Circuit(const QString& name);
    ~Circuit();
    const QString& name() const;
    DeviceType type();
    bool create(DeviceType type, const QList<Source *> &sources, SpiceModel *model);

    QList<Device *> getDevices(DeviceFlag flag = 0);

    const QList<Source*>& sources() const;
private:
    Device *addDeviceImpl(const QString& name, DeviceType type, SpiceModel *model = NULL);
    Device *addSourceImpl(const QString& name, DeviceType type,Source* source);
    void clean();

    DeviceType type_;
    QString name_;
    DeviceMap devices_;

};

}
}


#endif // CIRCUIT_H
