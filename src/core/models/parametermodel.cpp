#include "parametermodel.h"
#include <limits>
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


ParameterModel::ParameterModel() :
    paramId_(-1), libraryId_(-1),name_(QString()),
    initial_( std::numeric_limits<double>::max() ),
    minimum_( std::numeric_limits<double>::min() ),
    maximum_( std::numeric_limits<double>::max() ) {

}

ParameterModel &ParameterModel::setId(const int &id) {
    paramId_ = id;
    return *this;
}

ParameterModel &ParameterModel::setLibraryId(const int &libraryId) {
    libraryId_ = libraryId;
    return *this;
}

ParameterModel &ParameterModel::setName(const QString &name) {
    name_ = name;
    return *this;
}

ParameterModel &ParameterModel::setInitial(const double &initial) {
    initial_ = initial;
    return *this;
}

ParameterModel &ParameterModel::setMinimum(const double &minimum) {
    minimum_ = minimum;
    return *this;
}

ParameterModel &ParameterModel::setMaximum(const double &maximum) {
    maximum_ = maximum;
    return *this;
}
