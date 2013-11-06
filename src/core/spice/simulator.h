#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QList>
#include <QVariantMap>
namespace tsunami{
namespace core{

class Device;
class Source;
class Wire;

class SimulatorResult{
public:
    SimulatorResult(const QStringList& columns);
    SimulatorResult(const SimulatorResult& other);
    SimulatorResult& operator=(const SimulatorResult& other);
    void add( QVector<double>& row );
    void add( double i0, ... );

    void clear();

    int countRows();
    int countColumns();

private:
    QStringList columns_;
    QVector< QVector<double> > table_;
};

class Simulator {
public:
    Simulator();
    Simulator(const QString& path);

    void addDevice( const Device& device );
    void addSource( const Source& source );
    Device& device( const QString& name ) const;
    Source& source( const QString& name ) const;

    const QList<Device> devices() const;
    const QList<Source> sources() const;

    int countDevice();
    int countSource();

    const bool& external() const;
    const QString& path()  const;
    void external( bool external );
    void path( const QString& path );

    virtual bool simulate() = 0;

protected:
    virtual bool exec(const QStringList& arguments = QStringList());
    SimulatorResult data_;

    QString randomName();

private:
    bool external_;
    QString path_;


    QList<Device> devices_;
    QList<Source> sources_;
};

}
}

#endif // SIMULATOR_H
