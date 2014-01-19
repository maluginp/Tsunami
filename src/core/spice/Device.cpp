#include "Device.h"
#include "Terminal.h"
#include "SpiceModel.h"

namespace tsunami{
namespace spice{

Device::Device(const QString &name, DeviceType device)
    : GraphNode(name),
      numberPorts_(0),
      model_(NULL),
      device_(device),
      flags_(0x0) {

    switch(device_){
    case DEVICE_CAPACITOR:
    case DEVICE_RESISTOR:
    case DEVICE_DIODE:
    case DEVICE_VSOURCE:
    case DEVICE_ISOURCE:
        numberPorts_ = 2;
        break;
    case DEVICE_PBJT:
    case DEVICE_NBJT:
        numberPorts_ = 3;
        break;
    case DEVICE_NMOS:
    case DEVICE_PMOS:
    case DEVICE_PFET:
    case DEVICE_NFET:
        numberPorts_ = 4;
        break;
    case DEVICE_UNKNOWN:
    default:
        Q_ASSERT(false);
    }
}

void Device::source(Source source) {
//    if( source != SOURCE_UNKNOWN ){
        setFlag( DEVICE_FLAG_SOURCE  );
//        sourceOptions_ = options;
        source_ = source;
//    }
}

const Source &Device::source() const {
    return source_;
}

bool Device::isFlagged(DeviceFlag flag) {
    return ((flags_ & flag ) == flag);
}

void Device::setFlag(DeviceFlag flag) {
    flags_ = flag;
}

void Device::addFlag(DeviceFlag flag) {
    flags_ |= flag;
}

bool Device::hasModel() {
    return model_ != NULL;
}

void Device::setParameters(const QStringList &parameters) {
    parameters_ = parameters;

}

void Device::connect(Terminal *terminal) {
    link(terminal);
    terminal->attachDevice( this );
}

void Device::disconnect(Terminal *terminal) {
    terminal->detachDevice(  this );
    unlink(terminal);
}

void Device::setModel(SpiceModel *model) {
    Q_ASSERT( model != NULL );
    model_ = model;
    model_->typeDevice( device_ );
}

SpiceModel *Device::model() {
    Q_ASSERT( model_ != NULL );
    return model_;
}

QVector<Terminal *> Device::getTerminals() {
    QVector<GraphNode*> nodes = getNodes();
    QVector<Terminal*> terminals;
    int nNodes = nodes.size();
    for(int i=0; i < nNodes; ++i) {
        terminals.append( static_cast<Terminal*>(nodes[i]) );
    }

    return terminals;
}

Terminal *Device::terminal(int num) {
    Terminal* terminal = static_cast<Terminal*>(getNode( num ));
    return terminal;
}

QByteArray Device::netList() {
    QByteArray nets;

    QStringList temp;
    SourceMethod method = source_.method();

    if(method != SOURCE_METHOD_UNKNOWN){
        if(method == SOURCE_METHOD_CONST || method == SOURCE_METHOD_LINEAR){
            if(method == SOURCE_METHOD_CONST){
                temp.append( QString("%1").arg( source_.configuration("const").toDouble() ) );
            }else if(method == SOURCE_METHOD_LINEAR){
                temp.append( QString("%1").arg(source_.configuration("start").toDouble()) );
            }
        }
    }


    QString params = temp.join(" ");


    nets.append(name()).append(" ");
    nets.append(netListConnects());
    nets.append(params);

    if( model_ != 0 ){
        nets.append(QString(" %1").arg(model_->name()));
    }


    return nets;
}

QByteArray Device::sourceNetlist() {
    QByteArray netlist;
    SourceMethod method = source_.method();
    if( method == SOURCE_METHOD_LINEAR ){
        netlist.append( QString("%1 %2 %3 %4").arg(name())
                        .arg(source_.configuration("start").toDouble())
                        .arg(source_.configuration("end").toDouble())
                        .arg(source_.configuration("step").toDouble())
                       );
    }
    return netlist;
}

QByteArray Device::netListConnects() {
    QByteArray connects;
    for(int i=0; i < numberPorts(); ++i){
        if(terminal(i)->isRef()){
            connects.append("0");
        }else{
            connects.append( QByteArray::number(terminal(i)->id()) );
        }
        connects.append(" ");
    }
    return connects;
}



}
}
