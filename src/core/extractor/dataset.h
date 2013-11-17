#ifndef DATASET_H
#define DATASET_H
#include <QString>
#include <QList>
#include <QVariantMap>
//#include "models/measuremodel.h"
#include "defines.h"

namespace tsunami{
namespace db{
class MeasureModel;
}
namespace core{

class Dataset {
public:
    Dataset();

    bool load( AnalysisType type, DeviceType device,
               const QVariantMap& attr = QVariantMap() );


    void begin();
    bool isNext();
    const db::MeasureModel &next() const;
private:
    QList<db::MeasureModel> measures_;
    QList<db::MeasureModel>::iterator currentMeasure_;



};

}
}

#endif // DATASET_H
