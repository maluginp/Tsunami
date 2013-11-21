#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H
#include <QStringList>
#include <Model.h>
#include <QDate>

#include "defines.h"

namespace tsunami {
namespace db{

class DeviceModel : public Model {
public:

    void id( int deviceId ) { deviceId_ = deviceId; }
    void name( const QString& name) { name_ = name; }
    void type( DeviceType& type) { type_ = type; }
    void createAt(const QDateTime& createAt)  { createdAt_ = createAt; }
    void changeAt(const QDateTime& changeAt ) { changedAt_ = changeAt; }
    void enable( bool enable ) { enable_ = enable; }

    const int& id() const{ return deviceId_; }
    const QString& name() const { return name_; }
    const DeviceType& type() const { return type_; }
    const QDateTime& createAt() const { return createdAt_; }
    const QDateTime& changeAt() const { return changedAt_; }
    const bool& enable() const { return enable_; }

private:
    void testData();

    int deviceId_;
    QString name_;
    DeviceType type_;

    QDateTime createdAt_;
    QDateTime changedAt_;
    bool enable_;

};

}
}

#endif // DEVICEMODEL_H
