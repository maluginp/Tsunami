#include "simulator.h"
#include <QProcess>
namespace tsunami{
namespace core{


Simulator::Simulator()
    : external_(false)
    , circuit_(NULL) {
    Q_ASSERT(false);
}

Simulator::Simulator(const QString &path)
    : external_(true)
    , circuit_(NULL)
    , path_(path)
{


}

void Simulator::setCircuit(Circuit *circuit) {
    if(circuit != NULL){
        circuit_ = circuit;
    }
}

Circuit *Simulator::circuit() {
    Q_ASSERT(circuit_ != NULL);
    return circuit_;
}

const bool &Simulator::external() const {
    return external_;
}

const QString &Simulator::path() const {
    return path_;
}

void Simulator::external(bool external) {
    external_ = external;
}

void Simulator::path(const QString &path) {
    path_ = path;
}

bool Simulator::exec(QByteArray& data, const QStringList &arguments){

    QProcess process;
    process.setProcessChannelMode( QProcess::MergedChannels );
    // Start the process
    process.start(path(), arguments ,QIODevice::ReadWrite);

    data.clear();
    // Wait for it to start
    if(!process.waitForStarted())
        return false;

    // Continue reading the data until EOF reached
    while(process.waitForReadyRead())
        data.append(process.readAll());

    qDebug(data.data());

    return true;
}

QString Simulator::randomName(int num) {
    return QString("Mock");
}

}
}
