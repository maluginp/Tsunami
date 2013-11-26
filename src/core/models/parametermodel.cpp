#include "parametermodel.h"
#include <limits>
#include "defines.h"

namespace tsunami{
namespace db{
ParameterModel::ParameterModel() :
    libraryId_(-1),
    parameterId_(-1),
    initial_(.0),
    fitted_(.0),
    minimum_(.0),
    maximum_(.0),
    fixed_(false),
    enable_(false) {

}

ParameterModel::ParameterModel(const ParameterModel &other) :
    libraryId_(other.libraryId_),
    parameterId_(other.parameterId_),
    name_(other.name_),
    initial_(other.initial_),
    fitted_(other.fitted_),
    minimum_(other.minimum_),
    maximum_(other.maximum_),
    fixed_(other.fixed_),
    enable_(other.enable_) {
}

ParameterModel &ParameterModel::operator=(const ParameterModel &other) {
    libraryId_ = other.libraryId_;
    parameterId_ = other.parameterId_;
    name_ = other.name_;
    initial_ = other.initial_;
    fitted_ = other.fitted_;
    minimum_ = other.minimum_;
    maximum_ = other.maximum_;
    fixed_ = other.fixed_;
    enable_ = other.enable_;
    return *this;
}

}
}
