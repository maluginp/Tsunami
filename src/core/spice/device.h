#ifndef DEVICE_H
#define DEVICE_H

#include <QVariantMap>
#include <QString>
#include "graphnode.h"

namespace tsunami{
namespace core{

enum{
    DEVICE_NO_FLAG      = 0,
    DEVICE_FLAG_SOURCE  = 1 << 0
};
typedef unsigned long DeviceFlag;

class SpiceModel;
class Terminal;
class Device : public GraphNode  {
public:
    enum TypeDevice{
        UNKNOWN=-1,NBJT,PBJT,NFET,PFET,NMOS,PMOS,DIODE
    };
    enum TypeSource{
        SOURCE_UNKNOWN,
        SOURCE_CONST,
        SOURCE_AC,
        SOURCE_DC,
        SOURCE_TRAN
    };

    Device( const QString& name,TypeDevice device = UNKNOWN);

    void setSource( TypeSource source, const QVariantMap& options );

    bool isFlagged( DeviceFlag flag );
    void setFlag( DeviceFlag flag );
    void addFlag( DeviceFlag flag );

    void connect( Terminal* terminal );
    void disconnect( Terminal* terminal );
    void setModel( SpiceModel* model );
    SpiceModel* model();

    QVector<Terminal*> getTerminals();


private:
    SpiceModel* model_;
    TypeDevice device_;
    TypeSource source_;
    DeviceFlag flags_;
    QVariantMap sourceOptions_;
};

}
}
#endif // DEVICE_H
