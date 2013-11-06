#ifndef SPICEMODEL_H
#define SPICEMODEL_H

#include <QString>
#include <QVariantMap>

namespace tsunami{
namespace core{

class SpiceModel {
public:
    SpiceModel(const QString& name);
    SpiceModel& addParameter( const QString& name, const QVariant& value);
    SpiceModel& addParameter( const QVariantMap& parameters );

    const QString& name() const;
    void clear();
private:
    QString name_;
    QVariantMap parameters_;
};

}
}

#endif // SPICEMODEL_H
