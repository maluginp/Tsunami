#ifndef SOURCE_H
#define SOURCE_H

#include <QString>
#include <QVariantMap>

namespace tsunami{
namespace core{

class Source {
public:

    enum ModeSource{
        UNKNOWN = -1,
        VOLTAGE,
        CURRENT,
        GND
    };

    enum TypeOperation{
        NOT,
        AC,
        DC,
        TRAN
    };

    enum TypeSource{
        UNKNOWN,
        CONSTANT,
        LINEAR,
        NOLINEAR
    };

    Source();
    Source( const QString& name, ModeSource mode, TypeSource type,
            TypeOperation operation = NOT );

    Source& addParameter( const QString& name, const QVariant& value);
    Source& setNodes(int n, ...);

    Source( const Source& other);
    Source& operator=( const Source& other );

    QString netList();
private:
    QString name_;
    TypeSource type_;
    ModeSource mode_;
    TypeOperation operation_;

    QVector<uint> nodes_;

    QVariantMap parameters_;
};

}
}
#endif // SOURCE_H
