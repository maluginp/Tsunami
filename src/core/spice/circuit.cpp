#include "circuit.h"

#include "device.h"
#include "terminal.h"
#include "spicemodel.h"
#include <QMap>
#include <QDebug>
namespace tsunami{
namespace spice{



Circuit::Circuit(const QString &name)
    : name_(name)
    , flag_(DEVICE_NO_FLAG), mainDeviceId_(-1) {

     GraphNode::nextId_ = 1;
}


Circuit::~Circuit(){

}

void Circuit::typeAnalysis(AnalysisType analysis) {
    typeAnalysis_ = analysis;
}

const AnalysisType &Circuit::typeAnalysis() {
    return typeAnalysis_;
}

int Circuit::addDevice(const QString &name, DeviceType type) {
    Device* device = new Device(name,type);
    devices_[device->id()] = device;

    qDebug() << "Add device " << device->id() << " " << device->name();
//    devices_.insert( device->id(), device );
    return device->id();
}

void Circuit::removeDevice(int id) {
    DeviceMap::iterator it = devices_.find( id );
    if( it != devices_.end() ){
        Device* device = (*it).second;
        devices_.erase( it );
        delete device;
    }
}

Device *Circuit::getDevice(const QString &name) {
    DeviceMap::iterator it = devices_.begin();
    for(; it != devices_.end(); it++){
        Device* device = (*it).second;
        if(device->name().compare( name, Qt::CaseInsensitive ) == 0){
            return device;
        }
    }

    Q_ASSERT(false);

    return NULL;
}

Device *Circuit::getDevice(int deviceId) {
    DeviceMap::iterator it = devices_.find( deviceId );
    if( it != devices_.end() ){
        Device* device = (*it).second;
        return device;
    }

    Q_ASSERT(false);
    return NULL;
}

Device *Circuit::nextDevice() {
    while( currentDevice_ != devices_.end()){
        Device* device = (*currentDevice_).second;
        if(device->isFlagged(flag_)){
            break;
        }
        currentDevice_++;
    }
    if( currentDevice_ != devices_.end() ){
        Device* device = (*currentDevice_).second;
        currentDevice_++;
        return device;
    }
    return NULL;
}

Device *Circuit::mainDevice() {
    Q_ASSERT(mainDeviceId_ != -1);

    return getDevice(mainDeviceId_);

}

void Circuit::beginDevice(DeviceFlag flag) {
    flag_ = flag;
    currentDevice_ = devices_.begin();
}

int Circuit::addTerminal(const QString &name) {
    Terminal* terminal = new Terminal(name);
    terminals_[terminal->id()] = terminal;
//    .insert( terminal->id(), terminal );
    qDebug() << "Add terminal" << terminal->id() << " " << terminal->name() ;
    return terminal->id();
}

void Circuit::removeTerminal(int terminalId) {
    TerminalMap::iterator it = terminals_.find( terminalId );

    if(it != terminals_.end()){
        Terminal* terminal = (*it).second;
        terminals_.erase( it );
        delete terminal;
    }
}

Terminal *Circuit::getTerminal(const QString &name) {
    TerminalMap::iterator it = terminals_.begin();
    for(; it != terminals_.end(); it++){
        Terminal* terminal = (*it).second;
        if(terminal->name().compare( name, Qt::CaseInsensitive ) == 0){
            return terminal;
        }
    }

    Q_ASSERT(false);

    return NULL;

}

Terminal *Circuit::getTerminal(int terminalId) {
    TerminalMap::iterator it = terminals_.find( terminalId );
    if(it != terminals_.end()){
        Terminal* terminal = (*it).second;
        return terminal;
    }

    Q_ASSERT(false);
    return NULL;
}

Terminal *Circuit::nextTerminal() {

    if( currentTerminal_ != terminals_.end() ){

        Terminal* terminal = (*currentTerminal_).second;
        currentTerminal_++;
        return terminal;
    }

    Q_ASSERT(false);

    return NULL;
}

void Circuit::beginTerminal() {
    currentTerminal_ = terminals_.begin();
}

void Circuit::setSpiceModel(DeviceType deviceType, SpiceModel *model) {
    beginDevice();
    Device* device = nextDevice();
    while(device){
        if(device->type() == deviceType){
            device->setModel( model );
        }
        device = nextDevice();
    }
    addSpiceModel( model );
}

void Circuit::addSpiceModel(SpiceModel *model) {
    if( model == NULL || isModelExist(model->name()) ){
        return;
    }
    models_.append( model );
}

SpiceModel *Circuit::getModel(const QString &name) {
    ModelList::Iterator it = models_.begin();
    for(;it != models_.end(); it++){
        SpiceModel* model = static_cast<SpiceModel*>(*it);
        if(model->name().compare(name,Qt::CaseInsensitive) == 0){
            return model;
        }
    }

    Q_ASSERT(false);
    return NULL;
}

void Circuit::beginModel() {
    currentModel_ = models_.begin();
}

void Circuit::removeModel(const QString &name) {
    ModelList::iterator it = models_.begin();
    for(;it != models_.end(); it++){
        SpiceModel* model = static_cast<SpiceModel*>(*it);
        if(model->name().compare(name,Qt::CaseInsensitive) == 0){
            models_.erase( it );
            break;
        }
    }

    return;
}

SpiceModel *Circuit::nextModel() {
    if(currentModel_ != models_.end()){
        SpiceModel* model = static_cast<SpiceModel*>(*currentModel_);
        currentModel_++;
        return model;
    }

    return NULL;
}

void Circuit::setRefTerminal(int terminalId) {
    Terminal* terminal = getTerminal(terminalId);
    if(terminal != NULL && !terminal->isRef()){
        terminal->setRef();
    }
}

int Circuit::getRefTerminalId() {
    return 0;
}

void Circuit::connect(int deviceId, int terminalId) {
    Device* device = getDevice(deviceId);
    Terminal* terminal = getTerminal(terminalId);
    device->connect( terminal );
}

const QString &Circuit::name() const {
    return name_;
}

bool Circuit::isModelExist(const QString &name) {
    foreach(SpiceModel* model, models_){
        if(model->name().compare(name,Qt::CaseInsensitive) == 0){
            return true;
        }
    }
    return false;
}

QString Circuit::formSourceName(SourceMode mode, QString node) {
    QString name;
    switch(mode){
    case SOURCE_MODE_VOLTAGE: name="V"; break;
    case SOURCE_MODE_CURRENT: name="I"; break;
    default:
        return QString();
    }

    name.append( node.toLower() );
    return name;
}


bool Circuit::correct() {
    return true;
    // DC analysis
    if(typeAnalysis() == ANALYSIS_DC){
        int countSource = 0;
        beginDevice( DEVICE_FLAG_SOURCE );
        Device* device = nextDevice();

        while(device){
            if(device->getSource() == typeAnalysis()){
                countSource++;
            }

            device = nextDevice();
        }

        if(countSource > 2 || countSource < 1){
            return false;
        }
    }

    return true;

}

Circuit *Circuit::createCircuitDevice(DeviceType type, const QList<Source> &sources) {
    Circuit* circuit = new Circuit("Simulated");
    circuit->typeAnalysis( ANALYSIS_DC );

    circuit->sources_ = sources;

    if( type == DEVICE_NBJT || type == DEVICE_PBJT ){
//        if(sources.count() != 3){
//            Q_ASSERT(false);
//        }

        circuit->mainDeviceId_ = circuit->createBjt( circuit,type );
    }else if( type == DEVICE_NFET || type == DEVICE_PFET ){
        if(sources.count() != 4){
            Q_ASSERT(false);
        }
        circuit->mainDeviceId_  = circuit->createFet( circuit, type );
    }else if( type == DEVICE_NMOS || type == DEVICE_PMOS){
        if(sources.count() != 4){
            Q_ASSERT(false);
        }
        circuit->mainDeviceId_  = circuit->createMosfet(circuit, type);
    }else if( type == DEVICE_DIODE ){
        if(sources.count() != 2){
            Q_ASSERT(false);
        }
        circuit->mainDeviceId_  = circuit->createDiode( circuit,type );
    }else if(type== DEVICE_CAPACITOR){
        if(sources.count() != 2){
            Q_ASSERT(false);
        }
        circuit->mainDeviceId_  = circuit->createCap(circuit,type);
    }else if(type == DEVICE_RESISTOR){
        if(sources.count() != 2){
            Q_ASSERT(false);
        }
        circuit->mainDeviceId_ = circuit->createRes(circuit,type);
    }else{
        Q_ASSERT(false);
    }

    int termGnd = -1;
    // find ground
    foreach(Source source,sources){
        Terminal* term = circuit->getTerminal( source.node() );
        if( source.mode() == SOURCE_MODE_GND ){
            term->setRef();
            termGnd = term->id();
        }
    }

    Q_ASSERT(termGnd != -1);

    foreach(Source source,sources){
        if(source.mode() == SOURCE_MODE_GND || source.direction() != SOURCE_DIRECTION_INPUT){
            continue;
        }
        QString nameSource = formSourceName( source.mode(), source.node() );
        if(nameSource.isEmpty()) continue;

        int devId;
        if(source.mode() == SOURCE_MODE_VOLTAGE){
           devId = circuit->addDevice( nameSource, DEVICE_VSOURCE );
        }else if(source.mode() == SOURCE_MODE_CURRENT){
            devId = circuit->addDevice(nameSource, DEVICE_ISOURCE);
        }
        circuit->getDevice(devId)->setSource( source.method(), source.configuration() );

        circuit->connect( devId, circuit->getTerminal(source.node())->id()  );
        circuit->connect( devId, termGnd);

    }

    return circuit;
}

int Circuit::createBjt(Circuit* circuit, DeviceType type) {
    int bjtId = circuit->addDevice( "BJT", type );
    QStringList terms;
    terms << "E" << "B" << "C";

    // Terminals
    foreach(QString term, terms){
        circuit->connect( bjtId, circuit->addTerminal(term) );
    }

    return bjtId;
}

int Circuit::createRes(Circuit *circuit, DeviceType type) {
    int resId = circuit->addDevice("RES", type);

    QStringList terms;
    terms << "A" << "C";

    // Terminals
    foreach(QString term, terms) {
        circuit->connect( resId, circuit->addTerminal(term) );
    }

    return resId;
}

int Circuit::createCap(Circuit *circuit, DeviceType type) {
    int capId = circuit->addDevice("CAP", type);

    QStringList terms;
    terms << "A" << "C";

    // Terminals
    foreach(QString term, terms) {
        circuit->connect( capId, circuit->addTerminal(term) );
    }

    return capId;
}

int Circuit::createDiode(Circuit *circuit, DeviceType type) {
    int diodeId = circuit->addDevice("DIODE", type);

    QStringList terms;
    terms << "A" << "C";

    // Terminals
    foreach(QString term, terms) {
        circuit->connect( diodeId, circuit->addTerminal(term) );
    }

    return diodeId;
}

int Circuit::createFet(Circuit *circuit, DeviceType type) {
    int fetId = circuit->addDevice("FET", type);

    QStringList terms;
    terms << "S" << "G" << "D" << "B";

    // Terminals
    foreach(QString term, terms){
        circuit->connect( fetId, circuit->addTerminal(term) );
    }
    return fetId;
}

int Circuit::createMosfet(Circuit *circuit, DeviceType type) {
    int fetId = circuit->addDevice("MOSFET", type);

    QStringList terms;
    terms << "S" << "G" << "D" << "B";

    // Terminals
    foreach(QString term, terms){
        circuit->connect( fetId, circuit->addTerminal(term) );
    }
    return fetId;
}



}
}
