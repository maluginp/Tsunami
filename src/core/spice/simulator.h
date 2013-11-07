#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <QStringList>
namespace tsunami{
namespace core{

class Circuit;
class SpiceModel;

class Simulator {
public:
    Simulator();
    Simulator(const QString& path);

    void setCircuit( Circuit* circuit );
    Circuit* circuit();

    const bool& external() const;
    const QString& path()  const;
    void external( bool external );
    void path( const QString& path );

    virtual bool simulate() = 0;


protected:
    virtual bool exec(QByteArray &data,const QStringList& arguments = QStringList());
    virtual QByteArray generateNetListModels() = 0;
    virtual QByteArray generateNetList() = 0;

    QString randomName(int num);

private:
    bool external_;
    QString path_;
    Circuit* circuit_;

};

}
}

#endif // SIMULATOR_H
