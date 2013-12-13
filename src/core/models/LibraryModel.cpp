#include "LibraryModel.h"
#include "ParameterModel.h"
namespace tsunami{
namespace db{

void LibraryModel::setParameter(const QString &name, const ParameterModel &parameter) {

    int rows = parameters_.size();
    for(int row=0; row < rows; ++row){
        if(parameters_[row].name().compare(name, Qt::CaseInsensitive) == 0) {
            parameters_[row] = parameter;
            return;
        }
    }

    parameters_.append( parameter );

}

void LibraryModel::addEmptyParameter() {
    ParameterModel model;

    addParameter(model);
}

int LibraryModel::countParameters() {
    return parameters_.size();
}

ParameterModel &LibraryModel::find(const QString &name) {
    int rows = parameters_.size();

    int foundIndex = -1;
    for(int row=0; row < rows; ++row){
        if(parameters_[row].name().compare(name,Qt::CaseInsensitive) == 0){
            foundIndex = row;
            break;
        }
    }

    Q_ASSERT(foundIndex != -1);

    return find(foundIndex);
}

ParameterModel &LibraryModel::find(int index) {
     Q_ASSERT(index != -1);

    return parameters_[index];
}

//QVariant LibraryModel::parameterValue(const QModelIndex &index) const {
//    ParameterModel parameter = parameters_.value(index.row());
//    switch(index.column()){
//    case 0:  QVariant(parameter.id());
//    case 1: return QVariant(parameter.libraryId());
//    case 2: return QVariant(parameter.name());
//    case 3: return QVariant(parameter.initial());
//    case 4: return QVariant(parameter.minimum());
//    case 5: return QVariant(parameter.maximum());
//    }

//    return QVariant();
//}

LibraryModel::LibraryModel() :
    libraryId_(-1), deviceId_(-1), createdAt_(QDateTime::currentDateTime()),
    changedAt_(QDateTime::currentDateTime()),enable_(false)
{

}

LibraryModel::LibraryModel(const QString &name, int deviceId) :
    libraryId_(-1),name_(name),deviceId_(deviceId), createdAt_(QDateTime::currentDateTime()),
    changedAt_(QDateTime::currentDateTime()),enable_(false)
{

}

LibraryModel::LibraryModel(const LibraryModel &other) :
    libraryId_(other.libraryId_),deviceId_(other.deviceId_),
    name_(other.name_),createdAt_(other.createdAt_),changedAt_(other.changedAt_),
    enable_(other.enable_), parameters_(other.parameters_) {

}

LibraryModel &LibraryModel::operator=(const LibraryModel &other){

    libraryId_ = other.libraryId_;
    deviceId_ = other.deviceId_;
    name_ = other.name_;
    createdAt_ = other.createdAt_;
    changedAt_ = other.changedAt_;
    enable_ = other.enable_;
    parameters_ = other.parameters_;

    return *this;
}

const ParameterModel &LibraryModel::parameter(const QString &name) const {
    int rows = parameters_.size();

    int foundIndex = -1;
    for(int row=0; row < rows; ++row){
        if(parameters_[row].name().compare(name,Qt::CaseInsensitive) == 0){
            foundIndex = row;
            break;
        }
    }

    Q_ASSERT(foundIndex != -1);

    return parameter(foundIndex);
}

const ParameterModel &LibraryModel::parameter(int index) const {
    return parameters_[index];
}

ParameterModel &LibraryModel::parameter(const QString &name) {
    int rows = parameters_.size();

    int foundIndex = -1;
    for(int row=0; row < rows; ++row){
        if(parameters_[row].name().compare(name,Qt::CaseInsensitive) == 0){
            foundIndex = row;
            break;
        }
    }

    Q_ASSERT(foundIndex != -1);

    return parameter(foundIndex);
}

ParameterModel &LibraryModel::parameter(int index) {
    return parameters_[index];
}

const QList<ParameterModel> &LibraryModel::parameters() const {
    return parameters_;
}

const ParameterModel &LibraryModel::at(int id) const {
    return parameters_[id];
}

ParameterModel &LibraryModel::at(int index) {
    return parameters_[index];
}

void LibraryModel::removeParameters(const QList<int>& indexes) {
    QList<ParameterModel> parameters;
    int nParameter = 0;
    foreach(ParameterModel model, parameters_){
        if(!indexes.contains(nParameter)){
            parameters.append( model );
        }
        nParameter++;
    }

    parameters_ = parameters;

}

void LibraryModel::addParameter(const ParameterModel &parameter) {
    parameters_.append( parameter );
}

bool LibraryModel::parameterExists(const QString &name) {
    QList<ParameterModel>::iterator it = parameters_.begin();

    for(;it != parameters_.end(); ++it){
        if(it->name().compare(name,Qt::CaseInsensitive)){
            return true;
        }
    }

    return false;

}

}
}
