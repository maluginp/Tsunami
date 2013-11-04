#ifndef SOURCE_H
#define SOURCE_H

class Source {
public:
    Source( const Source& other);
    Source& operator=( const Source& other );
    // Constant
    Source( const QString& name, double constant  );
    // Linear
    Source( const QString& name, double start, double stop, double step );

    QString netList();
private:
    QString name_;
    TypeSource type_;

    QVariantList parameters_;
};
#endif // SOURCE_H
