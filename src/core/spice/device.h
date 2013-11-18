#ifndef DEVICE_H
#define DEVICE_H

#include <QVariantMap>
#include <QString>
#include <QStringList>
#include "graphnode.h"
#include "defines.h"

namespace tsunami{
namespace spice{


class SpiceModel;
class Terminal;
class Device : public GraphNode  {
public:

    Device( const QString& name,DeviceType device = DEVICE_UNKNOWN);

    void setSource( SourceMethod source, const QVariantMap& options );
    const SourceMethod& getSource() const;

    bool isFlagged( DeviceFlag flag );
    void setFlag( DeviceFlag flag );
    void addFlag( DeviceFlag flag );

    bool hasModel();

    void setParameters( const QStringList& parameters );

    void connect( Terminal* terminal );
    void disconnect( Terminal* terminal );
    void setModel( SpiceModel* model );
    SpiceModel* model();

    QVector<Terminal*> getTerminals();
    Terminal* terminal(int num);

    QByteArray netList();
    QByteArray sourceNetlist();

private:
    SpiceModel* model_;
    DeviceType device_;
    SourceMethod source_;
    DeviceFlag flags_;
    QVariantMap sourceOptions_;

    QStringList parameters_;
};

}
}
#endif // DEVICE_H
