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
    void analyses(const QVariantList& analyses);

    const bool& external() const;
    const QString& path()  const;
    void external( bool external );
    void path( const QString& path );

    virtual bool simulate() = 0;
    virtual db::MeasureModel *simulatedData();

    void typeAnalysis(AnalysisType analysisType);

    void measure(const db::MeasureModel* measure){
        measure_ = measure;
    }

protected:
    virtual bool exec(QByteArray &data,const QStringList& arguments = QStringList());

    QByteArray netlist();
    QString randomName(int num);

    const db::MeasureModel* measure_;
    db::MeasureModel* simulated_;
protected:
    QStringList columns_;
private:
    QByteArray netListModels();
    QByteArray netListPrints();
    QByteArray netListAnalyses();

    AnalysisType typeAnalysis_;
    QVariantList analyses_;

    bool external_;
    QString path_;
    Circuit* circuit_;




};

}
}

#endif // SIMULATOR_H
