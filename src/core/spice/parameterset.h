#ifndef PARAMETERSET_H
#define PARAMETERSET_H

#include <QString>
#include <float.h>
#include <QVector>
namespace tsunami{
namespace core{

struct Parameter{
    QString name;
    double value;
    double minimum;
    double maximum;
    bool fixed;
    Parameter() : name(QString()),value(0.0),minimum(DBL_MIN),maximum(DBL_MAX),
        fixed(false) {}
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
    ParameterSet(const QList<Parameter>& parameters);

    // Iterations
    void begin() const;
    void next() const;
    void previous() const;
    bool isEnd() const;

    void add( const QList<Parameter>& params );
    void add( const Parameter& param );
    bool remove( const QString& name );

    bool exists( const QString& name );

    void setValue(   const QString& name, double value );
    void setMinimum( const QString& name, double minimum );
    void setMaximum( const QString& name, double maximum );
    void setFixed(   const QString& name, bool fixed );

    const Parameter& operator()(const QString& name) const;
    const Parameter &param() const;
private:
    QList<Parameter> parameters_;
    mutable int it_;

    int findIndex(const QString& name) const;
};

}
}
#endif // PARAMETERSET_H
