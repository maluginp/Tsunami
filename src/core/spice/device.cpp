#include "device.h"
#include "terminal.h"
#include "spicemodel.h"

namespace tsunami{
namespace spice{

Device::Device(const QString &name, DeviceType device)
    : GraphNode(name)
    , device_(device)
    , model_(NULL)
    , flags_(0x0)
    , source_(SOURCE_UNKNOWN) {

}

void Device::setSource(TypeSource source, const QVariantMap &options) {
//    if( source != SOURCE_UNKNOWN ){
        setFlag( DEVICE_FLAG_SOURCE  );
        sourceOptions_ = options;
//    }
}

const TypeSource &Device::getSource() const {
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

    if(source_ != SOURCE_UNKNOWN){
        if(source_ == SOURCE_CONST && source_ == SOURCE_LINEAR){
            if(source_ == SOURCE_CONST){
                temp.append( QString("dc %1").arg( sourceOptions_.value("const").toDouble() ) );
            }else if(source_ == SOURCE_LINEAR){
                temp.append( QString("dc %1").arg(sourceOptions_.value("start").toDouble()) );
            }
        }
    }


    QString params = temp.join(" ");

    switch(device_){
    case DEVICE_CAPACITOR:
    case DEVICE_RESISTOR:
    case DEVICE_DIODE:
    case DEVICE_SOURCE:
        nets.append( QString("%1 %2 %3 %4 %5").arg(name())
                     .arg(terminal(0)->id()).arg(terminal(1)->id())
                     .arg(params).toAscii() );
        break;
    case DEVICE_PBJT:
    case DEVICE_NBJT:
    case DEVICE_NMOS:
    case DEVICE_PMOS:
    case DEVICE_PFET:
    case DEVICE_NFET:
        nets.append( QString("%1 %2 %3 %4 %5 %6 %7").arg(name())
                     .arg(terminal(0)->id())
                     .arg(terminal(1)->id())
                     .arg(terminal(2)->id())
                     .arg(terminal(3)->id())
                     .arg(params).toAscii() );
        break;
    case DEVICE_UNKNOWN:
    default:
        Q_ASSERT(false);
    }

    return nets;
}

QByteArray Device::sourceNetlist() {
    QByteArray netlist;
    if( source_ == SOURCE_LINEAR ){
        netlist.append( QString("%1 %2 %3 %4").arg(name())
                        .arg(sourceOptions_.value("start").toDouble())
                        .arg(sourceOptions_.value("end").toDouble())
                        .arg(sourceOptions_.value("step").toDouble())
                       );
    }
    return netlist;
}



}
}
