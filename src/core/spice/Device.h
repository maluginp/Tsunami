#ifndef DEVICE_H
#define DEVICE_H

#include "GraphNode.h"
#include "defines.h"
#include "Terminal.h"
#include "DeviceParameter.h"

namespace tsunami{
namespace spice{


class SpiceModel;

class TSUNAMI_EXPORT Device : public GraphNode  {
public:

    Device( const QString& name );
    void addTerminal(const QString& name);
    void addParameter( const QString& name, QVariant::Type type);


    bool isFlagged( DeviceFlag flag );
    void setFlag( DeviceFlag flag );
    void addFlag( DeviceFlag flag );
    void connect(const QString& node, Terminal* terminal);

    virtual DeviceType type() = 0;
    void setSpiceModel(SpiceModel* model);
    SpiceModel* spiceModel();
    bool hasSpiceModel();
//    void disconnect( Terminal* terminal );

    virtual QByteArray netlist() = 0;
    Terminal* terminal(const QString& node);

    const QList<DeviceParameter>& parameters() const;

protected:



    int numberTerminals_;
    QStringList terminals_;
    DeviceFlag flags_;
    SpiceModel* model_;

    QList<DeviceParameter> parameters_;

};

}
}
#endif // DEVICE_H
