#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <QString>
#include <QVariantMap>
#include "defines.h"
#include <map>
#include <QList>
namespace tsunami{
namespace spice{

class Terminal;
class Device;
class SpiceModel;

struct ltunsigned{
    bool operator()(const int& i1, const int& i2) const{
        return i1 < i2;
    }
};

typedef std::map<int,Device*> DeviceMap;
typedef std::map<int,Terminal*> TerminalMap;
typedef QList<SpiceModel*> ModelList;

class Circuit {
public:
    Circuit(const QString& name);
    ~Circuit();
    void typeAnalysis( AnalysisType analysis );
    const AnalysisType& typeAnalysis();
//    int addDevice( Device* device );
    int addDevice( const QString& name, DeviceType type );
    void removeDevice( int id );
    Device* getDevice( const QString& name );
    Device* getDevice( int deviceId );
    Device* nextDevice();
    Device* mainDevice();
    void beginDevice(DeviceFlag flag=DEVICE_NO_FLAG);

    int addTerminal( const QString& name );
    void removeTerminal( int terminalId );
    Terminal* getTerminal( const QString& name );
    Terminal* getTerminal( int terminalId );
    Terminal* nextTerminal();
    void beginTerminal();

    void setSpiceModel(DeviceType deviceType, SpiceModel* model);
    void addSpiceModel( SpiceModel* model );

    SpiceModel* getModel( const QString& name);
    void beginModel();
    void removeModel( const QString& name );
    SpiceModel* nextModel();

    void setRefTerminal( int terminalId );
    int getRefTerminalId();

    void connect(int deviceId, int terminalId);

    const QString& name() const;
    bool correct();

    static Circuit* createCircuitDevice(DeviceType type, const QList<Source>& sources);
//    static Circuit* createCircuitAC(DeviceType type);
//    static Circuit* createCircuitTran(DeviceType type);

    static QString formSourceName( SourceMode mode, QString node );

    QList<Source> sources() { return sources_; }
protected:
    QList<Source> sources_;
private:
    int createBjt(    Circuit *circuit, DeviceType type);
    int createRes(    Circuit* circuit, DeviceType type);
    int createCap(    Circuit* circuit, DeviceType type);
    int createDiode(  Circuit* circuit, DeviceType type);
    int createFet(    Circuit *circuit, DeviceType type);
    int createMosfet( Circuit *circuit, DeviceType type);

    bool isModelExist( const QString& name );



    QString name_;
    DeviceMap devices_;
    TerminalMap terminals_;
    ModelList models_;
    int mainDeviceId_;
    AnalysisType typeAnalysis_;


    DeviceFlag flag_;
    DeviceMap::iterator currentDevice_;
    TerminalMap::iterator currentTerminal_;
    ModelList::iterator  currentModel_;




};

}
}


#endif // CIRCUIT_H
