#ifndef DATASET_H
#define DATASET_H
#include <QString>
#include <QList>
#include <QVariantMap>
#include "models/measuremodel.h"
#include "defines.h"

namespace tsunami{
namespace core{

class Dataset {
public:
    Dataset();

    bool load( AnalysisType type, DeviceType device,
               const QVariantMap& attr = QVariantMap() );

    void begin();
    bool isNext();
    db::MeasureModel next();
private:
    QList<db::MeasureModel> measures_;
    QList<db::MeasureModel>::iterator currentMeasure_;



};

}
}


#endif // DATASET_H
