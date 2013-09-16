#include "parametermodel.h"

QVariantList ParameterModel::getColumns() {
    QVariantList columns;
    columns << "ID"
            << "Library ID"
            << "Parameter"
            << "Initial"
            << "Minimum"
            << "Maximum";
    return columns;
}
