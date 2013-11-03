#ifndef PARAMETERSET_H
#define PARAMETERSET_H

#include <QString>

struct Parameter{
    QString name;
    double value;
    double minimum;
    double maximum;
    bool fixed;

    Parameter(const Parameter& param) :
        name(param.name), value(param.value), minimum(param.minimum),
        maximum(param.maximum), fixed(param.fixed) {}

    Parameter &operator=(const Parameter& param){
        name    = param.name;
        value   = param.value;
        minimum = param.minimum;
        maximum = param.maximum;
        fixed   = param.fixed;
        return *this;
    }
};

class ParameterSet
{
public:
    ParameterSet();

    void addParameter( const Parameter& param );
    Parameter getParameter( const QString& name );

private:
    QVector<Parameter> parameters_;
};

#endif // PARAMETERSET_H
