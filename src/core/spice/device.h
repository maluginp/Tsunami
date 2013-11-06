#ifndef DEVICE_H
#define DEVICE_H

#include <QString>

namespace tsunami{
namespace core{

class ParameterSet;

class Device {
public:
    enum TypeDevice{
        UNKNOWN=-1,NBJT,PBJT,NFET,PFET,NMOS,PMOS,DIODE
    };

    Device();
    Device( const QString& name, TypeDevice type, const QString& model);
    Device( const Device& other );
    Device& operator=(const Device& other);

    void parameters( const ParameterSet& parameters );
    void model( const QString& model );
    void name( const QString& name );
    void type( TypeDevice type );

    const QString& model() const;
    const QString& name()  const;
    const ParameterSet& parameters() const;
    const TypeDevice& type() const;

private:
    QString name_;
    TypeDevice type_;
    QString model_;
    ParameterSet parameters_;
};

}
}
#endif // DEVICE_H
