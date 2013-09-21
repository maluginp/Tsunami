#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H
#include <QStringList>
#include <Model.h>

class DeviceModel : public Model
{
public:
    DeviceModel();
    DeviceModel(const DeviceModel& other);
    DeviceModel& operator=(const DeviceModel& other);


    inline const int& id() const                 { return deviceId_; }
    inline const QString& name() const           { return name_; }
    inline const QStringList& nodes() const      { return nodes_; }
    inline const QStringList& attributes() const { return attributes_; }

    DeviceModel& setId( const int& deviceId );
    DeviceModel& setName( const QString& name );
    DeviceModel& setNodes( const QStringList& nodes );
    DeviceModel& addNode( const QString& node );
    DeviceModel& setAttributes( const QStringList& attributes);
    DeviceModel& addAttribute( const QString& attribute );


private:
    int deviceId_;
    QString name_;
    QStringList nodes_;
    QStringList attributes_;
};

#endif // DEVICEMODEL_H
