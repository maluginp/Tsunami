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
    class ParameterStorage;
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
    Extractor(DeviceType type, int libraryId);

    void setDataset(Dataset* dataset);
    void setMethodOptimize(OptimizeBase* optimize);
    void setMethodOptimize(const QString& name);
    void setSimulator(spice::Simulator* simulator);

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
    const double& enable(const QString& name) const;
    const double& enable(int index) const;

    void value( const QString& name, double value );
    void value( int index, double value );
    void initial( const QString& name, double initial );
    void initial( int index, double initial );
    void minimum(const QString& name, double minimum);
    void minimum(int index, double minimum);
    void maximum(const QString& name, double maximum);
    void maximum(int index, double maximum);
    void fixed(const QString& name, bool fixed);
    void fixed(int index, bool fixed);
    void enable(const QString& name, bool enable);
    void enable(int index,bool enable);

    int countParameters();

    void run();

    virtual double functionError();

    const QVariant& config( const QString& key );
    void config(const QString& key, const QVariant& value);
    void config( const QVariantMap& config );


protected:
    QVariantMap configuration_;

    double subtract(double value1,double value2);
    virtual double computeError( db::MeasureModel* measured ) = 0;
    DeviceType type_;
    Dataset* dataset() { return dataset_; }
    spice::Simulator* simulator_;
    db::LibraryModel* library_;
    db::ParameterStorage* storage_;
private:
    OptimizeBase* optimize_;
    Dataset* dataset_;

};

}
}

#endif // EXTRACTOR_H
