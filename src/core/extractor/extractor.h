#ifndef EXTRACTOR_H
#define EXTRACTOR_H
#include <QObject>
#include <QList>

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
class Extractor : public QObject {
    Q_OBJECT
public:
    Extractor();

    void setDataset(Dataset* dataset);
    void setMethodOptimize();

    const double& value( const QString& name ) const;
    const double& initial( const QString& name ) const;
    const double& minimum(const QString& name) const;
    const double& maximum(const QString& name) const;
    const double& fixed(const QString& name) const;

    void run();


    virtual double functionError();
protected:

    virtual double computeError( db::MeasureModel* simulated, db::MeasureModel* measured ) = 0;

    QList<db::ParameterModel> parameters_;
    Dataset* dataset() { return dataset_; }
    spice::Simulator* simulator_;
    db::LibraryModel library_;
private:
    void libraryId_;

    Dataset* dataset_;

};

}
}

#endif // EXTRACTOR_H
