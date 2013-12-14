#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H
#include <QStringList>
#include <QDate>

#include "defines.h"

namespace tsunami {
namespace db{

class TSUNAMI_EXPORT DeviceModel {
public:
    DeviceModel(){}
    void id( int deviceId ) { deviceId_ = deviceId; }
    void name( const QString& name) { name_ = name; }
    void type( DeviceType type) { type_ = type; }
    void type( const QString& type);
    void createAt(const QDateTime& createAt)  { createdAt_ = createAt; }
    void changeAt(const QDateTime& changeAt ) { changedAt_ = changeAt; }
    void enable( bool enable ) { enable_ = enable; }
    void model( const QString& model) { model_ = model; }

    const int& id() const{ return deviceId_; }
    const QString& name() const { return name_; }
    const DeviceType& type() const { return type_; }
    QString typeJson() const;
    const QDateTime& createAt() const { return createdAt_; }
    const QDateTime& changeAt() const { return changedAt_; }
    const bool& enable() const { return enable_; }
    const QString& model() const { return model_; }

    QStringList nodes();

private:

    int deviceId_;
    QString name_;
    DeviceType type_;
    QString model_;

    QDateTime createdAt_;
    QDateTime changedAt_;
    bool enable_;

};

}
}

#endif // DEVICEMODEL_H
