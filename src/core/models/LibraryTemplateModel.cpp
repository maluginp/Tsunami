#include "LibraryTemplateModel.h"
#include "models/ParameterModel.h"
#include "models/DeviceModel.h"
#include "components/Json.h"
namespace tsunami {
namespace db{

QString LibraryTemplateModel::TEMPLATE_EXTENSION = QString("libtpl");
QString LibraryTemplateModel::TEMPLATE_DIRECTORY = QString("templates/");

LibraryTemplateModel::LibraryTemplateModel() {
}

void LibraryTemplateModel::devices(const QString &devicesJson) {
   QVariantList devices = QtJson::parse(devicesJson).toList();
   devices_.clear();
   if(devices.count() > 0){
       foreach(QVariant device,devices){
           DeviceType type =  DeviceModel::nameToType( device.toString() );
           if(type != DEVICE_UNKNOWN){
               devices_.append(type);
           }
       }
   }

   return;
}

void LibraryTemplateModel::parameters(const QString &parametersJson) {
    QVariantList parameters = QtJson::parse(parametersJson).toList();
    parameters_.clear();

    foreach(QVariant parameter, parameters){
        QVariantMap item = parameter.toMap();
        ParameterModel param;

        param.name( item.value("name").toString() );
        param.initial( item.value("initial").toDouble() );
        param.fitted( item.value("fitted").toDouble() );
        param.minimum( item.value("minimum").toDouble() );
        param.maximum( item.value("maximum").toDouble() );
        param.fixed( item.value("fixed").toDouble() );
        param.enable( item.value("enable").toBool() );

        parameters_.append( param );
    }
}

QString LibraryTemplateModel::devicesJson() {
    QVariantList devices;
    foreach(DeviceType type,devices_){
        devices.append( DeviceModel::typeToName(type) );
    }

    return QtJson::serializeStr( devices );
}

QString LibraryTemplateModel::parametersJson() {
    QVariantList parameters;
    foreach(ParameterModel param, parameters_){
        QVariantMap parameter;

        parameter.insert( "name", param.id() );
        parameter.insert("initial", param.initial());
        parameter.insert("fitted", param.fitted());
        parameter.insert("minimum", param.minimum());
        parameter.insert("maximum", param.maximum());
        parameter.insert("fixed", param.fixed());
        parameter.insert("enable", param.enable());

        parameters.append( parameter );

    }

    return QtJson::serializeStr(parameters);
}

bool LibraryTemplateModel::satisfyDevice(const QString &device) {
    DeviceType type = DeviceModel::nameToType(device);
    return satisfyDevice(type);
}

bool LibraryTemplateModel::satisfyDevice(DeviceType device) {
    return devices_.contains( device );

}

}
}
