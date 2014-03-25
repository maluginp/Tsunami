#ifndef EXTRACTORQUASINEWTON_H
#define EXTRACTORQUASINEWTON_H
#include "Extractor.h"
#include "math/Vector.h"
#include "math/Matrix.h"

namespace tsunami{
namespace core{

class TSUNAMI_EXPORT ExtractorQuasiNewton : public Extractor
{
public:
    ExtractorQuasiNewton(DeviceType type,
                         db::LibraryModel* library,
                         const QList<int>& measures);
public slots:
    void process();
private:
    Vector<double> modifyS(const Vector<double>& S,double cappa);
    Matrix<double> solveAk(const Matrix<double>& Ak, const Vector<double> &dGradient, const Vector<double> &dX);

    double newtonRaphsonMethod();
};

} // core
} // tsunami
#endif // EXTRACTORQUASINEWTON_H
