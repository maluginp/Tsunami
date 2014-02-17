#include "ParameterModel.h"
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

QString ParameterModel::debug() {
    QString debug = QString("Parameter ID:%1, LIBRARY_ID:%2, NAME:%3,"
                            "INITIAL:%4, FITTED:%5, MINIMUM:%6, MAXIMUM:%7,"
                            "FIXED:%8, ENABLE:%9")
            .arg(parameterId_).arg(libraryId_).arg(name_).arg(initial_)
            .arg(fitted_).arg(minimum_).arg(maximum_).arg(fixed_).arg(enable_);
    return debug;
}

}
}
