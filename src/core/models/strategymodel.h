#ifndef STRATEGYMODEL_H
#define STRATEGYMODEL_H
#include <QString>
#include "../models/parametermodel.h"

struct StrategyParameter{
    int    id;
    bool   fixed;
    double optimised;

    ParameterModel parameter;
};

class StrategyModel {
public:
    StrategyModel();

private:
    int strategyId_;
    QString name_;
    int libraryId_;

    QList<StrategyParameter> parameters_;

};

#endif // STRATEGYMODEL_H
