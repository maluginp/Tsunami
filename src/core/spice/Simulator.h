#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "defines.h"

namespace tsunami{
namespace db{
    class MeasureModel;
    class ParameterModel;
}
namespace spice{

class Circuit;
class SpiceModel;

class TSUNAMI_EXPORT Simulator {
public:
    Simulator();
    Simulator(const QString& path);

    void setCircuit( Circuit* circuit );
    Circuit* circuit();

    const bool& external() const;
    const QString& path()  const;
    void external( bool external );
    void path( const QString& path );

    virtual bool simulate() = 0;
    virtual db::MeasureModel *simulatedData();

protected:
    virtual bool exec(QByteArray &data,const QStringList& arguments = QStringList());

    QByteArray netlist();
    QString randomName(int num);


    db::MeasureModel* simulated_;
private:
    QByteArray netListModels();
    QByteArray netListPrints();
    QByteArray netListAnalysis();

    bool external_;
    QString path_;
    Circuit* circuit_;



};

}
}

#endif // SIMULATOR_H
