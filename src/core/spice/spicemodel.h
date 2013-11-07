#ifndef SPICEMODEL_H
#define SPICEMODEL_H

#include <QString>
#include <QVariantMap>
#include "defines.h"
namespace tsunami{
namespace core{

class SpiceModel {
public:
    SpiceModel(const QString& name,TypeDevice typeDevice = DEVICE_UNKNOWN);
    SpiceModel& addParameter( const QString& name, const QVariant& value);
    SpiceModel& addParameter( const QVariantMap& parameters );

    const QVariantMap& parameters() const;
    const QString& name() const;
    const TypeDevice& typeDevice() const;
    void typeDevice(TypeDevice type);
    void clear();
private:
    QString name_;
    QVariantMap parameters_;
    TypeDevice type_;
};

}
}

#endif // SPICEMODEL_H
