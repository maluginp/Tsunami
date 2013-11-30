#ifndef SPICEMODEL_H
#define SPICEMODEL_H

#include <QString>
#include <QVariantMap>
#include "defines.h"
namespace tsunami{
namespace db{
class LibraryModel;
}
namespace spice{


struct ConstraintSpiceParameter{

    double min;
    double max;
    bool fixed;

};

class SpiceModel {
public:
    SpiceModel(const QString& name,DeviceType typeDevice = DEVICE_UNKNOWN);
    void setLibrary(db::LibraryModel* library);


    const QString& name() const;
    const DeviceType& typeDevice() const;
    void typeDevice(DeviceType type);

    QByteArray generateNetList();

    bool isFixed(const QString& parameter);
    ConstraintSpiceParameter constraint( const QString& parameter );

private:
    QString name_;
    DeviceType type_;
    db::LibraryModel* library_;
};

}
}

#endif // SPICEMODEL_H
