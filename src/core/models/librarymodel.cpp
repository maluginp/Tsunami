#include "librarymodel.h"
#include "parametermodel.h"
LibraryModel::LibraryModel()
{
}

void LibraryModel::updateLibraryId(const int &libraryId) {
    int rows = parameters_.size();
    for(int row=0; row < rows; ++row){
        parameters_[row].libraryId_ = libraryId;
    }
    libraryId_ = libraryId;
}

bool LibraryModel::parameterExists(const QString &parameter) {
    foreach(ParameterModel param, parameters_){
        if(param.name() == parameter){
            return true;
        }
    }

    return false;
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

QString LibraryModel::parameterName(const int &row) const {
    return QString();
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

QList<ParameterModel> LibraryModel::parameters() {
    return parameters_;
}
