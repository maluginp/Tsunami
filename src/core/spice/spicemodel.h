#ifndef SPICEMODEL_H
#define SPICEMODEL_H

#include <QString>
#include <QVariantMap>
#include "defines.h"
namespace tsunami{
namespace core{


struct ConstraintSpiceParameter{

    double min;
    double max;
    bool fixed;

};

class SpiceModel {
public:
    SpiceModel(const QString& name,TypeDevice typeDevice = DEVICE_UNKNOWN);
    void add( const QString& name, const QVariant& value);
    void add( const QVariantMap& parameters );

    void set( const QString& name, const QVariant& value );
    QVariant get( const QString& name );

    void setConstraints( const QMap<QString, ConstraintSpiceParameter>& constraints );

    const QVariantMap& parameters() const;
    const QString& name() const;
    const TypeDevice& typeDevice() const;
    void typeDevice(TypeDevice type);
    void clear();

    bool isFixed(const QString& parameter);
    ConstraintSpiceParameter constraint( const QString& parameter );

private:
    QString name_;
    QVariantMap parameters_;
    TypeDevice type_;
    QMap<QString, ConstraintSpiceParameter> constraints_;
};

}
}

#endif // SPICEMODEL_H
