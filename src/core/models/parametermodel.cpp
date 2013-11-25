#include "parametermodel.h"
#include <limits>


namespace tsunami{
namespace db{

ParameterModel::ParameterModel() {
}

ParameterModel::ParameterModel(const ParameterModel &other) {
    libraryId_ = other.libraryId_;
    parameterId_ = other.parameterId_;
    name_ = other.name_;
    initial_ = other.initial_;
    fitted_ = other.fitted_;
    minimum_ = other.minimum_;
    maximum_ = other.maximum_;
    fixed_ = other.fixed_;
    enable_ = other.enable_;
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
