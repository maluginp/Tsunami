#ifndef EXTRACTORGLOBAL_H
#define EXTRACTORGLOBAL_H
#include "extractor.h"
namespace tsunami{
namespace core{

class ExtractorGlobal : public Extractor {
public:
    ExtractorGlobal();
protected:
    double computeError(db::MeasureModel *measure);
};

}
}
#endif // EXTRACTORGLOBAL_H
