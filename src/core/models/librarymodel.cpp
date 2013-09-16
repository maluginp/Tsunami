#include "librarymodel.h"
#include "models/parametermodel.h"
LibraryModel::LibraryModel()
{
}


QVariant LibraryModel::parameterValue(const int &row, const int &column) const {
    ParameterModel parameter = parameters_.value(row);
    switch(column){
    case 0:  QVariant(parameter.id());
    case 1: return QVariant(parameter.libraryId());
    case 2: return QVariant(parameter.name());
    case 3: return QVariant(parameter.initial());
    case 4: return QVariant(parameter.minimum());
    case 5: return QVariant(parameter.maximum());
    }

    return QVariant();
}

bool LibraryModel::setParameterValue(const int &row, const int &column, const QVariant &value) {
    bool ok;

    switch(column){
    case 0: parameters_[row].id()        = value.toInt(&ok);break;
    case 1: parameters_[row].libraryId() = value.toInt(&ok); break;
    case 2: parameters_[row].name()      = value.toString();
    case 3: parameters_[row].initial()   = value.toDouble(&ok); break;
    case 4: parameters_[row].minimum()   = value.toDouble(&ok); break;
    case 5: parameters_[row].maximum()   = value.toDouble(&ok); break;
    default:
        return false;
    }

    return true;
}
