#include "circuit.h"

#include "device.h"
#include "terminal.h"
#include "spicemodel.h"
namespace tsunami{
namespace spice{



Circuit::Circuit(const QString &name)
    : name_(name)
    , flag_(DEVICE_NO_FLAG)
{

}

void Circuit::typeAnalysis(TypeAnalysis analysis) {
    typeAnalysis_ = analysis;
}

const TypeAnalysis &Circuit::typeAnalysis() {
    return typeAnalysis_;
}

int Circuit::addDevice(const QString &name, DeviceType type) {
    Device* device = new Device(name,type);

    devices_.insert( device->id(), device );
    return device->id();
}

void Circuit::removeDevice(int id) {
    DeviceMap::Iterator it = devices_.find( id );
    if( it != devices_.end() ){
        Device* device = static_cast<Device*>(it.value());
        devices_.erase( it );
        delete device;
    }
}

Device *Circuit::getDevice(const QString &name) {
    DeviceMap::Iterator it = devices_.begin();
    for(; it != devices_.end(); it++){
        Device* device = static_cast<Device*>(it.value());
        if(device->name().compare( name, Qt::CaseInsensitive ) == 0){
            return device;
        }
    }

    Q_ASSERT(false);

    return NULL;
}

Device *Circuit::getDevice(int deviceId) {
    DeviceMap::Iterator it = devices_.find( deviceId );
    if( it != devices_.end() ){
        Device* device = static_cast<Device*>(it.value());
        return device;
    }

    Q_ASSERT(false);
    return NULL;
}

Device *Circuit::nextDevice() {
    while( currentDevice_ != devices_.end() &&
           !(static_cast<Device*>(currentDevice_.value())->isFlagged(flag_))) {
        currentDevice_++;
    }
    if( currentDevice_ != devices_.end() ){
        Device* device = static_cast<Device*>(currentDevice_.value());
        currentDevice_++;
        return device;
    }

    Q_ASSERT(false);

    return NULL;
}

void Circuit::beginDevice(DeviceFlag flag) {
    flag_ = flag;
    currentDevice_ = devices_.begin();
}

int Circuit::addTerminal(const QString &name) {
    Terminal* terminal = new Terminal(name);
    terminals_.insert( terminal->id(), terminal );

    return terminal->id();
}

void Circuit::removeTerminal(int terminalId) {
    TerminalMap::Iterator it = terminals_.find( terminalId );

    if(it != terminals_.end()){
        Terminal* terminal = static_cast<Terminal*>(it.value());
        terminals_.erase( it );
        delete terminal;
    }
}

Terminal *Circuit::getTerminal(const QString &name) {
    TerminalMap::Iterator it = terminals_.begin();
    for(; it != terminals_.end(); it++){
        Terminal* terminal = static_cast<Terminal*>(it.value());
        if(terminal->name().compare( name, Qt::CaseInsensitive ) == 0){
            return terminal;
        }
    }

    Q_ASSERT(false);

    return NULL;

}

Terminal *Circuit::getTerminal(int terminalId) {
    TerminalMap::Iterator it = terminals_.find( terminalId );
    if(it != terminals_.end()){
        Terminal* terminal = static_cast<Terminal*>(it.value());
        return terminal;
    }

    Q_ASSERT(false);
    return NULL;
}

Terminal *Circuit::nextTerminal() {

    if( currentTerminal_ != terminals_.end() ){

        Terminal* terminal = static_cast<Terminal*>(currentTerminal_.value());
        currentTerminal_++;
        return terminal;
    }

    Q_ASSERT(false);

    return NULL;
}

void Circuit::beginTerminal() {
    currentTerminal_ = terminals_.begin();
}

void Circuit::addSpiceModel(const QString &name, const QVariantMap &parameters) {

    if( isModelExist(name) ){
        return;
    }

    SpiceModel* model = new SpiceModel( name );
    if(parameters.size() > 0){
        model->add( parameters );
    }

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
    ModelList::Iterator it = models_.begin();
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

    if( type == DEVICE_NBJT || type == DEVICE_PBJT ){
        if(sources.count() != 3){
            Q_ASSERT(false);
        }

        createBjt( circuit,type );
    }else if( type == DEVICE_NFET || type == DEVICE_PFET ){
        if(sources.count() != 4){
            Q_ASSERT(false);
        }
        createFet( circuit, type );
    }else if( type == DEVICE_NMOS || type == DEVICE_PMOS){
        if(sources.count() != 4){
            Q_ASSERT(false);
        }
        createMosfet(circuit, type);
    }else if( type == DEVICE_DIODE ){
        if(sources.count() != 2){
            Q_ASSERT(false);
        }
        createDiode( circuit,type );
    }else if(type== DEVICE_CAPACITOR){
        if(sources.count() != 2){
            Q_ASSERT(false);
        }
        createCap(circuit,type);
    }else if(type == DEVICE_RESISTOR){
        if(sources.count() != 2){
            Q_ASSERT(false);
        }
        createRes(circuit,type);
    }else{
        Q_ASSERT(false);
    }

    int termGnd = -1;
    // find ground
    foreach(Source source,sources){
        Terminal* term = circuit->getTerminal( source.node );
        if( source.mode == SOURCE_MODE_GND ){
            term->setRef();
            termGnd = term->id();
        }
    }

    Q_ASSERT(termGnd != -1);

    foreach(Source source,sources){
        if(source.mode == SOURCE_MODE_GND){
            continue;
        }
        QString nameSource = formSourceName( source.mode, source.node );
        if(nameSource.isEmpty()) continue;

        int devId = circuit->addDevice( nameSource, DEVICE_SOURCE );
        circuit->getDevice(devId)->setSource( source.method, source.configuration );

        circuit->connect( devId, circuit->getTerminal(source.node)->id()  );
        circuit->connect( devId, termGnd);

    }
}

void Circuit::createBjt(Circuit* circuit, AnalysisType type) {
    int bjtId = circuit->addDevice( "BJT", type );
    QStringList terms;
    terms << "E" << "B" << "C";

    // Terminals
    foreach(QString term, terms){
        circuit->connect( bjtId, circuit->addTerminal(term); );
    }


}

void Circuit::createRes(Circuit *circuit, AnalysisType type) {
    int resId = circuit->addDevice("RES", type);

    QStringList terms;
    terms << "A" << "C";

    // Terminals
    foreach(QString term, terms) {
        circuit->connect( resId, circuit->addTerminal(term); );
    }
}

void Circuit::createCap(Circuit *circuit, AnalysisType type) {
    int capId = circuit->addDevice("CAP", type);

    QStringList terms;
    terms << "A" << "C";

    // Terminals
    foreach(QString term, terms) {
        circuit->connect( capId, circuit->addTerminal(term); );
    }
}

void Circuit::createDiode(Circuit *circuit, AnalysisType type) {
    int fetId = circuit->addDevice("DIODE", type);

    QStringList terms;
    terms << "A" << "C";

    // Terminals
    foreach(QString term, terms) {
        circuit->connect( fetId, circuit->addTerminal(term); );
    }

}

void Circuit::createFet(Circuit *circuit, AnalysisType type) {
    int fetId = circuit->addDevice("FET", type);

    QStringList terms;
    terms << "S" << "G" << "D" << "B";

    // Terminals
    foreach(QString term, terms){
        circuit->connect( fetId, circuit->addTerminal(term); );
    }

}

void Circuit::createMosfet(Circuit *circuit, AnalysisType type) {
    int fetId = circuit->addDevice("MOSFET", type);

    QStringList terms;
    terms << "S" << "G" << "D" << "B";

    // Terminals
    foreach(QString term, terms){
        circuit->connect( fetId, circuit->addTerminal(term); );
    }

}



}
}
