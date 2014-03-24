#ifndef EXTRACTORQUASINEWTON_H
#define EXTRACTORQUASINEWTON_H
#include "Extractor.h"
namespace tsunami{
namespace core{

class TSUNAMI_EXPORT ExtractorQuasiNewton : public Extractor
{
public:
    ExtractorQuasiNewton(DeviceType type, db::LibraryModel* library, const QList<int>& measures);
public slots:
    void process();
};

} // core
} // tsunami
#endif // EXTRACTORQUASINEWTON_H
