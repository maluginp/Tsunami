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

    bool updateParameters(const QString& library, QList<db::ParameterModel> parameters );

    virtual bool simulate() = 0;
    virtual db::MeasureModel *simulatedData();

protected:
    virtual bool exec(QByteArray &data,const QStringList& arguments = QStringList());
    virtual QByteArray generateNetListModels() = 0;
    virtual QByteArray generateNetList() = 0;
    virtual QByteArray generateNetPrints() = 0;
    QString randomName(int num);


    db::MeasureModel* simulated_;
private:
    bool external_;
    QString path_;
    Circuit* circuit_;



};

}
}

#endif // SIMULATOR_H
