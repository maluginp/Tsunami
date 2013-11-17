#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <QString>
#include <QVariantMap>
#include "defines.h"

namespace tsunami{
namespace spice{

class Terminal;
class Device;
class SpiceModel;

typedef QMap<int,Device*> DeviceMap;
typedef QMap<int,Terminal*> TerminalMap;
typedef QList<SpiceModel*> ModelList;

class Circuit {
public:
    Circuit(const QString& name);

    void typeAnalysis( TypeAnalysis analysis );
    const TypeAnalysis& typeAnalysis();
//    int addDevice( Device* device );
    int addDevice( const QString& name, DeviceType type );
    void removeDevice( int id );
    Device* getDevice( const QString& name );
    Device* getDevice( int deviceId );
    Device* nextDevice();
    void beginDevice(DeviceFlag flag=DEVICE_NO_FLAG);

    int addTerminal( const QString& name );
    void removeTerminal( int terminalId );
    Terminal* getTerminal( const QString& name );
    Terminal* getTerminal( int terminalId );
    Terminal* nextTerminal();
    void beginTerminal();

    void addSpiceModel(const QString& name,
                        const QVariantMap& parameters = QVariantMap() );
    void addSpiceModel( SpiceModel* model );

    SpiceModel* getModel( const QString& name);
    void beginModel();
    void removeModel( const QString& name );
    SpiceModel* nextModel();

    void setRefTerminal( int terminalId );

    void connect(int deviceId, int terminalId);

    const QString& name() const;
    bool correct();
protected:

private:
    bool isModelExist( const QString& name );


    QString name_;
    DeviceMap devices_;
    TerminalMap terminals_;
    ModelList models_;

    TypeAnalysis typeAnalysis_;

    DeviceFlag flag_;
    DeviceMap::Iterator currentDevice_;
    TerminalMap::Iterator currentTerminal_;
    ModelList::Iterator  currentModel_;
};

}
}


#endif // CIRCUIT_H
