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

    bool create(DeviceType type, SourceManager *sources );

    QList<Device *> getDevices(DeviceFlag flag = 0);

private:
    Device *addDeviceImpl(const QString& name, DeviceType type);
    void clean();

    QString name_;
    DeviceMap devices_;
//    ModelList models_;

//    DeviceMap::iterator currentDevice_;
//    ModelList::iterator  currentModel_;




};

}
}


#endif // CIRCUIT_H
