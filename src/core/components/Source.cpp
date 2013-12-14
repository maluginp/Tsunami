#include "source.h"

#include <QStringList>
#include "Log.h"
namespace tsunami{
Source::Source(const QString &_node, SourceMode _mode, SourceDirection _direction, SourceMethod _method, const QVariantMap &_configuration) {
    node_ = _node;
    mode_ = _mode;
    method_ = _method;
    configuration_ = _configuration;
    direction_ = _direction;
}
Source::Source(const Source &other) {
    node_ = other.node_;
    mode_ = other.mode_;
    method_ = other.method_;
    configuration_ = other.configuration_;
    direction_ = other.direction_;
}

Source &Source::operator=(const Source &other) {
    node_ = other.node_;
    mode_ = other.mode_;
    method_ = other.method_;
    configuration_ = other.configuration_;
    direction_ = other.direction_;
    return *this;

}

QString Source::name(bool upper) {
    QString name;

    if(mode_ == SOURCE_MODE_VOLTAGE || mode_==SOURCE_MODE_GND){
        name = "V";
    }else if(mode_ == SOURCE_MODE_CURRENT){
        name = "I";
    }else{
        Q_ASSERT(false);
    }
    if(upper){
        name = name.append(node_).toUpper();
    }else{
        name.append( node_.toLower() );
    }

    return name;
}

QString Source::title( const QString& format ) const {
    QString title;

    if( format.isEmpty()){
        title = methodJson().toUpper();

        QStringList configs;
        foreach(QString key, configuration_.keys()){
            configs.append( QString("%1=%2").arg(key)
                            .arg(configuration_.value(key).toString()) );
        }

        title = QString("%1 (%2)").arg(title).arg(configs.join(";"));
    }else{
        title = format;
        title.replace( "%node", node() ).replace("%NODE", node().toUpper() );
        title.replace( "%method", methodJson()).replace("%METHOD",methodJson().toUpper());
        title.replace( "%mode", modeJson() ).replace("%MODE", modeJson().toUpper());
        title.replace( "%dir", directionJson()).replace("%DIR",directionJson().toUpper());
        log::logDebug() << "Format: " << format << " Result: " << title;
    }
    return title;
}

void Source::addConfiguration(const QString &key, const QVariant &value) {
    configuration_.insert(key,value);
}

QString Source::directionJson() const {
    if(direction_ == SOURCE_DIRECTION_INPUT){
        return "input";
    }else if(direction_ == SOURCE_DIRECTION_OUTPUT){
        return "output";
    }
    Q_ASSERT(false);
}

void Source::direction(const QString &_direction) {
    if(_direction.compare("input") == 0){
        direction_ = SOURCE_DIRECTION_INPUT;
    }else if(_direction.compare("output") == 0){
        direction_ = SOURCE_DIRECTION_OUTPUT;
    }else{
        Q_ASSERT(false);
    }
}

void Source::method(const QString &_method) {
    if(_method.compare("linear") == 0){
        method_ = SOURCE_METHOD_LINEAR;
    }else if(_method.compare("list") == 0){
        method_ = SOURCE_METHOD_LIST;
    }else if(_method.compare("const") == 0){
        method_ = SOURCE_METHOD_CONST;
    }else{
        method_ = SOURCE_METHOD_UNKNOWN;
    }
}

void Source::mode(const QString &_mode) {
    if(_mode.compare("voltage")==0){
        mode_ = SOURCE_MODE_VOLTAGE;
    }else if(_mode.compare("current") ==0){
        mode_ = SOURCE_MODE_CURRENT;
    }else if(_mode.compare("gnd") == 0){
        mode_ = SOURCE_MODE_GND;
    }else{
        Q_ASSERT(false);
    }
}

QString Source::methodJson() const{
    if(method_ == SOURCE_METHOD_CONST){
        return "const";
    }else if(method_ == SOURCE_METHOD_LINEAR){
        return "linear";
    }else if(method_ == SOURCE_METHOD_LIST){
        return "list";
    }

//    Q_ASSERT(false);
    return "";
}

QVariant Source::configuration(const QString &key,const QVariant& defaultValue) const{
    if(configuration_.contains(key)){
        return configuration_[key];
    }

    return defaultValue;
}

Source::Source() {
    mode_ = SOURCE_MODE_GND;
    method_ = SOURCE_METHOD_UNKNOWN;
    direction_ = SOURCE_DIRECTION_INPUT;

}

QString Source::modeJson() const{
    if(mode_ == SOURCE_MODE_VOLTAGE){
        return "voltage";
    }else if(mode_ == SOURCE_MODE_CURRENT){
        return "current";
    }else if(mode_==SOURCE_MODE_GND){
        return "gnd";
    }
    Q_ASSERT(false);
}


}
