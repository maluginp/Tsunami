#ifndef MEASUREPLOTMODEL_H
#define MEASUREPLOTMODEL_H
#include <QVariant>
#include <Model.h>

class MeasurePlotModel : public Model
{
public:
    MeasurePlotModel();

private:
    QVariant plotX_;
    QVariant plotY_;
    QVariant grouped_;
};

#endif // MEASUREPLOTMODEL_H
