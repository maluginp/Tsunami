#ifndef SOURCE_H
#define SOURCE_H
#include <QVariantMap>
#include "defines.h"
namespace tsunami{

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

    QVariant configuration(const QString& key, const QVariant &defaultValue = QVariant()) const;
    bool hasConfiguration( const QString& key ) { return configuration_.contains(key); }
    void addConfiguration(const QString& key, const QVariant& value);

    QString directionJson() const;

    void node(const QString& node) { node_ = node; }
    void mode(SourceMode mode) { mode_ = mode; }
    void mode(const QString& _mode);
    void method(SourceMethod method) { method_ = method; }
    void method(const QString& _method);
    void configurations(const QVariantMap& configurations) { configuration_ = configurations; }
    void direction(SourceDirection direction) { direction_ = direction; }
    void direction(const QString& _direction);

    static bool compare( const QList<Source>& sources1,
                         const QList<Source>& sources2 );

    bool operator==(const Source& other);

    Source( const QString& _node, SourceMode _mode,
            SourceDirection _direction = SOURCE_DIRECTION_INPUT,
            SourceMethod _method = SOURCE_METHOD_UNKNOWN,
            const QVariantMap& _configuration = QVariantMap());


    Source(const Source& other);
    Source& operator=(const Source& other);

    QString name( bool upper = false );
    QString title( const QString& format = QString() ) const;
private:
    SourceMode mode_;
    SourceMethod method_;
    QVariantMap configuration_;
    QString node_;
    SourceDirection direction_;
};
}

#endif // SOURCE_H
