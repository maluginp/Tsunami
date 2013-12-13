#ifndef DEVICE_H
#define DEVICE_H

#include "GraphNode.h"
#include "defines.h"

namespace tsunami{
namespace spice{


class SpiceModel;
class Terminal;
class  Device : public GraphNode  {
public:

    Device( const QString& name,DeviceType device = DEVICE_UNKNOWN);

    void source( Source source );
    const Source &source() const;

    bool isFlagged( DeviceFlag flag );
    void setFlag( DeviceFlag flag );
    void addFlag( DeviceFlag flag );

    bool hasModel();

    void setParameters( const QStringList& parameters );
    DeviceType type() { return device_; }
    void connect( Terminal* terminal );
    void disconnect( Terminal* terminal );
    void setModel( SpiceModel* model );
    SpiceModel* model();

    QVector<Terminal*> getTerminals();
    Terminal* terminal(int num);

    QByteArray netList();
    QByteArray sourceNetlist();

    int numberPorts() { return numberPorts_; }

private:
    int numberPorts_;
    SpiceModel* model_;
    DeviceType device_;
    Source source_;
    DeviceFlag flags_;

    QStringList parameters_;
};

}
}
#endif // DEVICE_H
