#ifndef SIMULATOR_H
#define SIMULATOR_H

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

    void simulate() = 0;

private:
    bool external_;
    QString path_;

    QList<Device> devices_;
    QList<Source> sources_;
};

#endif // SIMULATOR_H
