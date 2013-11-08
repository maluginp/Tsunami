#include "librarymodel.h"
#include "parametermodel.h"

LibraryModel::LibraryModel() :
    libraryId_(-1),projectId_(-1),userId_(-1),createAt_(QDateTime::currentDateTime()),
    changeAt_(QDateTime::currentDateTime()),enable_(false)
{
}

LibraryModel::~LibraryModel()
{
}

LibraryModel &LibraryModel::setId(const int &id) {
    libraryId_ = id;

    int rows = parameters_.size();
    for(int row=0; row < rows; ++row){
        parameters_[row].setLibraryId(id);
    }


    return *this;
}

LibraryModel &LibraryModel::setName(const QString &name) {
    name_ = name;
    return *this;
}

LibraryModel &LibraryModel::setProjectId(const int &projectId) {
    projectId_ = projectId;
    return *this;
}

LibraryModel &LibraryModel::setUserId(const int &userId) {
    userId_ = userId;
    return *this;
}

LibraryModel &LibraryModel::setCreateAt(const QDateTime &createAt) {
    createAt_ = createAt;
    return *this;
}

LibraryModel &LibraryModel::setChangeAt(const QDateTime &changeAt) {
    changeAt_ = changeAt;
    return *this;
}

LibraryModel &LibraryModel::setEnable(const bool &enable) {
    enable_ = enable;
    return *this;
}

void LibraryModel::setParameter(const QString &parameter, const ParameterModel &model) {

    int rows = parameters_.size();
    for(int row=0; row < rows; ++row){
        if(parameters_[row].name() == parameter){
            parameters_[row] = model;
            return;
        }
    }

    parameters_ << model;

}


bool LibraryModel::parameterExists(const QString &parameter) const {
    foreach(ParameterModel param, parameters_){
        if(param.name() == parameter){
            return true;
        }
    }

    return false;
}


QVariant LibraryModel::parameterValue(const QModelIndex &index) const {
    ParameterModel parameter = parameters_.value(index.row());
    switch(index.column()){
    case 0:  QVariant(parameter.id());
    case 1: return QVariant(parameter.libraryId());
    case 2: return QVariant(parameter.name());
    case 3: return QVariant(parameter.initial());
    case 4: return QVariant(parameter.minimum());
    case 5: return QVariant(parameter.maximum());
    }

    return QVariant();
}

bool LibraryModel::setParameterValue(const QModelIndex &index, const QVariant &value) {
    bool ok;

    switch(index.column()){
    case 0: parameters_[index.row()].setId(        value.toInt(&ok)   ); break;
    case 1: parameters_[index.row()].setLibraryId( value.toInt(&ok)   ); break;
    case 2: parameters_[index.row()].setName(      value.toString()   ); break;
    case 3: parameters_[index.row()].setInitial(   value.toDouble(&ok)); break;
    case 4: parameters_[index.row()].setMinimum(   value.toDouble(&ok)); break;
    case 5: parameters_[index.row()].setMaximum(   value.toDouble(&ok)); break;
    default:
        return false;
    }

    return true;
}

const ParameterModel &LibraryModel::parameter(const QString &param) const {
    int rows = parameters_.size();
    int foundIndex = -1;
    for(int row=0; row < rows; ++row){
        if(parameters_[row].name() == param){
            foundIndex = row;
            break;
        }
    }

    return parameter( foundIndex );
}

const ParameterModel &LibraryModel::parameter(const int &nParameter) const {

    int rows = parameters_.size();
    Q_ASSERT(nParameter >= 0 || nParameter < rows);

    return parameters_[nParameter];

}

QList<ParameterModel> LibraryModel::parameters() const {
    return parameters_;
}


