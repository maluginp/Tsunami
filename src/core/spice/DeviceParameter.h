#ifndef DEVICEPARAMETER_H
#define DEVICEPARAMETER_H
#include "defines.h"

namespace tsunami{
namespace spice{

class TSUNAMI_EXPORT DeviceParameter {
public:
    DeviceParameter(const QString& name, QVariant::Type type);

    bool enabled();
    void enable(bool enabled = true);
    void value(const QVariant& value);
    const QVariant& value() const;
    const QString& name() const;
    QString title();
private:
    QString name_;
    QVariant::Type type_;
    QVariant value_;
    bool enable_;
};


}
}

#endif // DEVICEPARAMETER_H
