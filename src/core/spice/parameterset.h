#ifndef PARAMETERSET_H
#define PARAMETERSET_H

#include <QString>
#include <float.h>

struct Parameter{
    QString name;
    double value;
    double minimum;
    double maximum;
    bool fixed;

    Parameter( const QString& p_name, double p_value=0.0, double p_minimum=DBL_MIN,
               double p_maximum=DBL_MAX, bool p_fixed=false) :
        name(p_name), value(p_value),minimum(p_minimum),
        maximum(p_maximum), fixed(p_fixed) { }

    Parameter(const Parameter& param) :
        name(param.name), value(param.value), minimum(param.minimum),
        maximum(param.maximum), fixed(param.fixed) { }

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

    void add( const Parameter& param );
    bool remove( const QString& name );

    void setValue(   const QString& name, double value );
    void setMinimum( const QString& name, double minimum );
    void setMaximum( const QString& name, double maximum );
    void setFixed(   const QString& name, bool fixed );

    Parameter& operator()(const QString& name);

private:
    QVector<Parameter> parameters_;
};
#endif // PARAMETERSET_H
