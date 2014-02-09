#ifndef SOURCE_H
#define SOURCE_H
#include <QVariantMap>
#include "defines.h"
namespace tsunami{
namespace spice{
class DeviceParameter;
}
enum SourceMethod{
    SOURCE_METHOD_UNKNOWN,
    SOURCE_METHOD_CONST,
    SOURCE_METHOD_LINEAR,
    SOURCE_METHOD_LIST
};
enum SourceMode{
    SOURCE_MODE_VOLTAGE,
    SOURCE_MODE_CURRENT,
    SOURCE_MODE_GND
};

enum SourceDirection{
    SOURCE_DIRECTION_INPUT,
    SOURCE_DIRECTION_OUTPUT
};

enum SourceType{
    SOURCE_TYPE_GND,
    SOURCE_TYPE_CONST,
    SOURCE_TYPE_PULSE,
    SOURCE_TYPE_EXP,
    SOURCE_TYPE_SIN
};

class TSUNAMI_EXPORT Source{
public:

    Source();

    const QString& node() const { return node_; }
    const SourceMode& mode() const { return mode_; }
    QString modeJson() const;
    const SourceMethod& method() const {return method_; }
    QString methodJson() const;
    const SourceDirection& direction() const {return direction_; }
    const QVariantMap& configurations() const {return configuration_; }

    void type(const QString& type);
    void type(SourceType type);
    const SourceType& type();
    QString typeJson();

    QVariant configuration(const QString& key, const QVariant &defaultValue = QVariant()) const;
    bool hasConfiguration( const QString& key ) { return configuration_.contains(key); }
    void addConfiguration(const QString& key, const QVariant& value);
    void addConfiguration( const QPair<QString,QVariant>& pair);
    void cleanConfiguration();

    QString directionJson() const;

    void node(const QString& node) { node_ = node; }
    void mode(SourceMode mode) { mode_ = mode; }
    void mode(const QString& _mode);
    void method(SourceMethod method) { method_ = method; }
    void method(const QString& method);
    void configurations(const QVariantMap& configurations) { configuration_ = configurations; }
    void direction(SourceDirection direction) { direction_ = direction; }
    void direction(const QString& _direction);

    static bool compare( const QList<Source>& sources1,
                         const QList<Source>& sources2 );


    bool operator==(const Source& other);

    Source(const QString& node, SourceMode mode,
            SourceDirection direction = SOURCE_DIRECTION_INPUT,
            SourceMethod method = SOURCE_METHOD_UNKNOWN,
            const QVariantMap& configuration = QVariantMap());

    QVariantMap json() const;

    Source(const Source& other);
    Source& operator=(const Source& other);

    bool isPositive();

    QString name( bool upper = false );
    QString title( const QString& format = QString() ) const;

    bool isGnd()        { return mode_ == SOURCE_MODE_GND; }
    bool isCurrent()    { return mode_ == SOURCE_MODE_CURRENT; }
    bool isVoltage()    { return mode_ == SOURCE_MODE_VOLTAGE; }
private:
    SourceMode mode_;
    SourceMethod method_;
    QVariantMap configuration_;
    QString node_;
    SourceDirection direction_;
    SourceType type_;
};

class TSUNAMI_EXPORT SourceManager{
public:
    QList<Source*> inputs();
    QList<Source*> outputs();
    const Source* inputByNode(const QString& name);
private:
    QList<Source *> sources_;
};

}

#endif // SOURCE_H
