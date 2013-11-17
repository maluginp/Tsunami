#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H
#include <QStringList>
#include <Model.h>

#include "defines.h"

namespace tsunami {
namespace db{

class DeviceModel : public Model {
public:

    void id( int deviceId ) { deviceId_ = deviceId; }
    void name( const QString& name) { name_ = name; }
    void type( DeviceType& type) { type_ = type; }
    void createAt(const QDate& createAt)  { createdAt_ = createAt; }
    void changeAt(const QDate& changeAt ) { changedAt_ = changeAt; }
    void enable( bool enable ) { enable_ = enable; }

    const int& id() const{ return deviceId_; }
    const QString& name() const { return name_; }
    const DeviceType& type() const { return type_; }
    const QDate& createAt() const { return createdAt_; }
    const QDate& changeAt() const { return changedAt_; }
    const bool& enable() const { return enable_; }

private:
    int deviceId_;
    QString name_;
    DeviceType type_;

    QDate& createdAt_;
    QDate& changedAt_;
    bool enable_;

};

}
}

#endif // DEVICEMODEL_H
