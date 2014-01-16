#include "LibraryTemplateModel.h"
#include "models/ParameterModel.h"
#include "models/LibraryModel.h"
#include "models/DeviceModel.h"
#include "components/Json.h"

#include <limits>
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

void LibraryTemplateModel::addDevice(const QString &deviceName) {
    DeviceType deviceType = DeviceModel::nameToType(deviceName);
    if(deviceType != DEVICE_UNKNOWN){
        addDevice( deviceType );
    }
}

void LibraryTemplateModel::addDevice(const QStringList &deviceNames) {
    foreach(QString deviceName, deviceNames){
        deviceName = deviceName.trimmed();
        addDevice(deviceName);
    }
}

void LibraryTemplateModel::addDevice(DeviceType device) {
    devices_.append( device );
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

void LibraryTemplateModel::addParameter(const ParameterModel &parameter) {
    parameters_.append( parameter );
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




        parameter.insert("name", param.name() );
        parameter.insert("initial", checkInfinity(param.initial()));
        parameter.insert("fitted",  checkInfinity(param.fitted()));
        parameter.insert("minimum", checkInfinity(param.minimum()));
        parameter.insert("maximum", checkInfinity(param.maximum()));
        parameter.insert("fixed",   param.fixed());
        parameter.insert("enable",  param.enable());

        parameters.append( parameter );

    }
    bool ok;
    QString json = QtJson::serializeStr(parameters,ok);

    return json;
}

LibraryModel *LibraryTemplateModel::convertToLibraryModel() {
    LibraryModel* library = new LibraryModel();

    library->id(-1);
    library->name( name_ );
    library->addParameter( parameters_ );

    library->enable( true );
    return library;
}

bool LibraryTemplateModel::satisfyDevice(const QString &device) {
    DeviceType type = DeviceModel::nameToType(device);
    return satisfyDevice(type);
}

bool LibraryTemplateModel::satisfyDevice(DeviceType device) {
    return devices_.contains( device );

}

QList<LibraryTemplateModel *> LibraryTemplateModel::parseDirectory() {

    QList<LibraryTemplateModel *> templateModels;
    LibraryTemplateModel* templateModel;

    QDir dir(TEMPLATE_DIRECTORY);

    QStringList filter;
    filter << QString("*.%1").arg(TEMPLATE_EXTENSION);

    QFileInfoList templates = dir.entryInfoList( filter, QDir::Files );

    foreach(QFileInfo templateFileInfo, templates){
        QFile templateFile( templateFileInfo.filePath());

        if(templateFile.open(QIODevice::ReadOnly)){
            QXmlStreamReader reader( &templateFile );
            templateModel = new LibraryTemplateModel();
            while(!reader.atEnd()){
                QXmlStreamReader::TokenType token = reader.readNext();
                if(token != QXmlStreamReader::StartElement){
                    continue;
                }

                if(reader.name() == "library") {
                    QXmlStreamAttributes attrs = reader.attributes();

                    QStringList devices =  attrs.value("devices").toString()
                                                .split(",",QString::SkipEmptyParts);

                    templateModel->addDevice(devices);
                    templateModel->name( attrs.value("name").toString() );

                }else if(reader.name() == "parameter"){
                    QXmlStreamAttributes attrs = reader.attributes();

                    ParameterModel parameter;
                    parameter.name( attrs.value("name").toString() );
                    parameter.initial(attrs.value("initial").toString().toDouble());
                    parameter.fitted(  parameter.initial() );
                    parameter.minimum( attrs.value("minimum").toString().toDouble() );
                    parameter.maximum( attrs.value("maximum").toString().toDouble());
                    parameter.fixed( false );
                    parameter.enable( true );


                    templateModel->addParameter( parameter );

                }

            }
            templateModels.append( templateModel );
        }else{
//            qDebug() << templateFile.errorString();
        }
    }

    return templateModels;
}

QVariant LibraryTemplateModel::checkInfinity(double value) {
    if(value == TSUNAMI_DOUBLE_INF){
        return QVariant("inf");
    }
    return value;
}

}
}
