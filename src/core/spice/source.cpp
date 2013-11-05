#include "source.h"
namespace tsunami{
namespace core{

Source &Source::setNodes(int n,...) {
    nodes_.clear();

    va_list vl;
    va_start(vl,n);
    for (i=0;i<n;i++) {
        nodes_ << va_arg(vl,uint);
    }
    va_end(vl);
    return *this;
}

Source::Source(const Source &other) :
    name_(other.name_), type_(other.type_), mode_(other.mode_),
    operation_(other.operation_),nodes_(other.nodes_),parameters_(other.parameters_)
{

}

Source &Source::operator=(const Source &other) {
    name_       = other.name_;
    type_       = other.type_;
    mode_       = other.mode_;
    operation_  = other.operation_;
    nodes_      = other.nodes_;
    parameters_ = other.parameters_;
    return *this;
}

QString Source::netList() {

    QString net;

}

Source::Source()
    : name_(QString()), type_(UNKNOWN),mode_(UNKNOWN),operation_(NOT) {
}

Source::Source(const QString &name, Source::ModeSource mode,
               Source::TypeSource type, Source::TypeOperation operation)
    : name_(name),mode_(mode),type_(type),operation_(operation)
{



}

Source &Source::addParameter(const QString &name, const QVariant &value) {
    parameters_.insert( name,value );
    return *this;
}



}
}
