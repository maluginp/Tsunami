#include "parameterset.h"

namespace tsunami{
namespace core{

ParameterSet::ParameterSet() : it_(0) { }

ParameterSet::ParameterSet(const QList<Parameter> &parameters) :
    parameters_(parameters),it_(0) {

}
void ParameterSet::begin() const { it_ = 0; }
void ParameterSet::next() const { it_++; }
void ParameterSet::previous() const { it_--; }

bool ParameterSet::isEnd() const {
    return (it_ >= parameters_.size() || it_ < 0);
}

void ParameterSet::add(const QList<Parameter> &params) {
    int nParameters = params.size();
    for(int i=0; i < nParameters; ++i ) {
        if(!exists(params[i].name)) {
            parameters_.append( params[i] );
        }
    }
}

void ParameterSet::add(const Parameter &param) {
    if(!exists(param.name)){
        parameters_.append(param);
    }
}

bool ParameterSet::remove(const QString &name) {
    int it = it_;
    begin();
    while(isEnd()){
        if(param().name.compare(name) == 0){
            parameters_.removeAt(it_);
            if(it >= it_){
                it--;
            }
        }
    }

    it_ = it;
}

bool ParameterSet::exists(const QString &name) {
    return (findIndex(name) != -1);
}

void ParameterSet::setValue(const QString &name, double value) {
    int it = findIndex(name);

    if(it != -1){
        parameters_[it].value = value;
    }

}

void ParameterSet::setMinimum(const QString &name, double minimum) {
    int it = findIndex(name);

    if(it != -1){
        parameters_[it].minimum = minimum;
    }
}

void ParameterSet::setMaximum(const QString &name, double maximum) {
    int it = findIndex(name);

    if(it != -1){
        parameters_[it].maximum = maximum;
    }
}

void ParameterSet::setFixed(const QString &name, bool fixed) {
    int it = findIndex(name);

    if(it != -1){
        parameters_[it].fixed = fixed;
    }

}

const Parameter &ParameterSet::operator()(const QString &name) const {
    int it = findIndex(name);

    Q_ASSERT( it != -1 );

    return parameters_[it];
}

const Parameter &ParameterSet::param() const {
    Q_ASSERT( it_ >= 0 && it_ < parameters_.size() );

    return parameters_[it_];
}


int ParameterSet::findIndex(const QString &name) const{
    int it = it_;
    int foundIndex = -1;
    begin();
    while(isEnd()){
        if(param().name.compare(name) == 0){
            foundIndex = it_;
            break;
        }
        next();
    }
    it_ = it;

    return foundIndex;
}

}
}

