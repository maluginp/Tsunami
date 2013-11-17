#ifndef EXTRACTOR_H
#define EXTRACTOR_H
#include <QObject>
#include <QList>
#include"defines.h"

namespace tsunami{
namespace db{
    class MeasureModel;
    class ParameterModel;
    class LibraryModel;
}
namespace spice{
    class Simulator;
}

namespace core{
class Dataset;
class OptimizeBase;

class Extractor : public QObject {
    Q_OBJECT
public:
    Extractor(DeviceType type);

    void setDataset(Dataset* dataset);
    void setMethodOptimize(OptimizeBase* optimize);
    const double& value( const QString& name ) const;
    const double& value( int index ) const;
    const double& initial( const QString& name ) const;
    const double& initial( int index ) const;
    const double& minimum(const QString& name) const;
    const double& minimum(int index) const;
    const double& maximum(const QString& name) const;
    const double& maximum(int index) const;
    const double& fixed(const QString& name) const;
    const double& fixed(int index) const;

    void run();

    void value( const QString& name, double value );
    virtual double functionError();

protected:
    QVariantMap configuration_;

    double subDataValue(double value1,double value2);
    virtual double computeError( db::MeasureModel* measured ) = 0;
    DeviceType type_;
    QList<db::ParameterModel> parameters_;
    Dataset* dataset() { return dataset_; }
    spice::Simulator* simulator_;
    db::LibraryModel library_;
private:
    void libraryId_;
    OptimizeBase* optimize_;
    Dataset* dataset_;

};

}
}

#endif // EXTRACTOR_H
