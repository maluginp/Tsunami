#ifndef EXTRACTOR_H
#define EXTRACTOR_H
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

class TSUNAMI_EXPORT Extractor : public QObject{
    Q_OBJECT
public:
    enum Tolerance{
        TOLERANCE_FUNCTION,
        TOLERANCE_STEP,
        TOLERANCE_GRADIENT
    };

    Extractor(DeviceType type, db::LibraryModel* library, const QList<int>& measures );

    db::LibraryModel* library();
    core::OptimizeBase* methodOptimize();

    void methodOptimize(const QString& name);
    void simulator(spice::Simulator* simulator);
    void simulator(const QString& simulator, const QString &path);
    int numberParameters();



    virtual double functionError();

    const QVariant& config( const QString& key );
    void config(const QString& key, const QVariant& value);
    void config( const QVariantMap& config );

//    const bool& enable(int index) const;
//    const double& initial(int index) const;
    const double& fitted(int index) const;
    const double& minimum(int index) const;
    const double& maximum(int index) const;
    const bool& fixed(int index) const;

//    void enable(int index, bool enable);
//    void initial(int index,double initial);
    void fitted(int index,double fitted);
//    void minimum(int index,double minimum);
//    void maximum(int index,double maximum);
//    void fixed(int index,bool fixed);
//    void value(int index,double value);

    double step(int index) const;
    void step(int index,double step);
    static Extractor* createExtractor(const QString& methodOptimization,
                                      DeviceType type, db::LibraryModel* library,
                                      const QList<int>& measures  );
public slots:
    virtual void process() = 0;
    virtual void stop();
protected:
    bool testBoundary( int index, double value );
    void increaseIteration();

    virtual bool checkConvergence(bool showMessage = true);
    void currentFunctionError( double funcError );
    const double& currentFunctionError( );
    void saveSteps();

    QVariantMap configuration_;

    double subtract(double value1,double value2);
    virtual double computeError(const db::MeasureModel* measure );


    double previousFunctionError_, currentFunctionError_;
    QMap<int, double> previousSteps_,currentSteps_;

    DeviceType type_;
    spice::Simulator* simulator_;
    db::LibraryModel* library_;
    OptimizeBase* optimize_;
    Dataset* dataset_;

    QMap<Tolerance,double> tolerances_;

    int maxIterations_;
    int iteration_;

    // DB STORAGES
    db::ParameterStorage* storageParameters_;
    bool stopped_;
signals:
    void log(const QString& name);
    void finished();
//    void updateFunctionError();

};

}
}

#endif // EXTRACTOR_H
