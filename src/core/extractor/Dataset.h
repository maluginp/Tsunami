#ifndef DATASET_H
#define DATASET_H
#include <QString>
#include <QList>
#include "defines.h"

namespace tsunami{
namespace db{
class MeasureModel;
}

typedef QList<db::MeasureModel*> ListMeasures;

namespace core{

class TSUNAMI_EXPORT Dataset {
public:
    Dataset();

    bool load( AnalysisType type, DeviceType device,
               const QVariantMap& attr = QVariantMap() );

    void load(const QList<int>& measureIds );

    void begin();
    bool isNext();
    const db::MeasureModel *next();
private:
    ListMeasures measures_;
    int measureIt_;

};

}
}

#endif // DATASET_H
