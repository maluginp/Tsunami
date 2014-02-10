#include "Source.h"

#include <QStringList>
#include "Log.h"
namespace tsunami{
Source::Source(const QString &node, SourceMode mode, SourceDirection direction,
               SourceMethod method, const QVariantMap &configuration) :
    mode_(SOURCE_MODE_GND),
    method_(SOURCE_METHOD_UNKNOWN),
    direction_(SOURCE_DIRECTION_INPUT)
{
    direction_ = direction;
    node_ = node;
    mode_   = mode;
    if( direction_ == SOURCE_DIRECTION_INPUT ){
        method_ = method;
        configuration_ = configuration;
    }
}

QVariantMap Source::json() const{
    QVariantMap sourceJson;
    sourceJson.insert("node",      node_ );
    sourceJson.insert("mode",      modeJson());
    sourceJson.insert("type",      typeJson());
    sourceJson.insert("direction", directionJson());

    QVariantMap config;
    if( mode_ != SOURCE_MODE_GND ){
        foreach( QString key, configuration_.keys() ){
            config.insert( key, configuration_.value(key) );
        }
    }

    sourceJson.insert( "configuration", config );

    return sourceJson;
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

bool Source::isPositive() {

    if(method_ == SOURCE_METHOD_CONST){
        double constant = configuration_.value("const").toDouble();
        return (constant >= .0);
    }else if(method_ == SOURCE_METHOD_LINEAR){
        double stop = configuration_.value("end").toDouble();
        return (stop >= .0);
    }

    return true;
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
        title.replace( "%node", node().toLower() ).replace("%NODE", node().toUpper() );
        if(mode_ == SOURCE_MODE_GND || direction_ == SOURCE_DIRECTION_OUTPUT){
            title.replace( "%method", "",Qt::CaseInsensitive);
        }else{
            title.replace( "%method", methodJson().toLower()).replace("%METHOD",methodJson().toUpper());
        }
        title.replace( "%mode", modeJson().toLower() ).replace("%MODE", modeJson().toUpper());
        title.replace( "%dir", directionJson().toLower()).replace("%DIR",directionJson().toUpper());

        if(title.contains("%config",Qt::CaseInsensitive)){
            if(!configuration_.isEmpty()){
                QStringList configs;
                foreach(QString key, configuration_.keys()){
                    configs.append( QString("%1=%2").arg(key)
                                    .arg(configuration_.value(key).toString()) );
                }

                title.replace("%config", configs.join(" ").toLower() );
                title.replace("%CONFIG", configs.join(" ").toUpper());
            }else{
                title.replace("%config","",Qt::CaseInsensitive);
            }
        }

//        log::logDebug() << QString("Formating title source:\n"
//                                   "Input: %1\nOutput: %2")
//                           .arg(format).arg(title);
    }
    return title;
}

void Source::addConfiguration(const QString &key, const QVariant &value) {
    configuration_.insert(key,value);
}

void Source::addConfiguration(const QPair<QString, QVariant> &pair) {
    addConfiguration(pair.first,pair.second);
}

void Source::cleanConfiguration() {
    configuration_.clear();
}

QString Source::directionJson() const {
    if(direction_ == SOURCE_DIRECTION_INPUT){
        return "input";
    }else if(direction_ == SOURCE_DIRECTION_OUTPUT){
        return "output";
    }
    Q_ASSERT(false);
    return QString();
}

void Source::direction(const QString &_direction) {
    if(_direction.compare("input", Qt::CaseInsensitive) == 0){
        direction_ = SOURCE_DIRECTION_INPUT;
    }else if(_direction.compare("output", Qt::CaseInsensitive) == 0){
        direction_ = SOURCE_DIRECTION_OUTPUT;
    }else{
        Q_ASSERT(false);
    }
}

void Source::method(const QString &method) {
    if(method.compare("linear", Qt::CaseInsensitive) == 0){
        method_ = SOURCE_METHOD_LINEAR;
    }else if(method.compare("list", Qt::CaseInsensitive) == 0){
        method_ = SOURCE_METHOD_LIST;
    }else if(method.compare("const", Qt::CaseInsensitive) == 0){
        method_ = SOURCE_METHOD_CONST;
    }else{
        method_ = SOURCE_METHOD_UNKNOWN;
    }
}

void Source::mode(const QString &_mode) {
    if(_mode.compare("voltage", Qt::CaseInsensitive)==0){
        mode_ = SOURCE_MODE_VOLTAGE;
    }else if(_mode.compare("current", Qt::CaseInsensitive) ==0){
        mode_ = SOURCE_MODE_CURRENT;
    }else if(_mode.compare("gnd", Qt::CaseInsensitive) == 0){
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

void Source::type(const QString &type) {
    if(type == "const"){
        type_ = SOURCE_TYPE_CONST;
    }else if(type == "pulse"){
        type_ = SOURCE_TYPE_PULSE;
    }else if(type == "exp"){
        type_ = SOURCE_TYPE_EXP;
    }else if(type == "sin"){
        type_ = SOURCE_TYPE_SIN;
    }
}

void Source::type(SourceType type) {
    type_ = type;
}

const SourceType &Source::type() {
    return type_;
}

QString Source::typeJson() const {
    if(type_ == SOURCE_TYPE_CONST){
        return "const";
    }else if(type_ == SOURCE_TYPE_PULSE){
        return "pulse";
    }else if(type_ == SOURCE_TYPE_EXP){
        return "exp";
    }else if(type_ == SOURCE_TYPE_SIN){
        return "sin";
    }

    Q_ASSERT(false);
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
    type_ = SOURCE_TYPE_CONST;
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
    return QString();
}

bool Source::operator==(const Source& other){
    // Общие свойства источника
    bool res =  (node_ == other.node_)
                && (mode_ == other.mode_)
                && (direction_ == other.direction_);

//    if (!res)  {
//        return false;
//    }
    bool ok;
    if (mode_ == SOURCE_MODE_GND || direction_ == SOURCE_DIRECTION_OUTPUT) {
        return res;
    }

    res &= (method_ == other.method_);

    if(!res) {
        return false;
    }

    if( method_ == SOURCE_METHOD_CONST ){

        res = (configuration_.count() == other.configuration_.count() )
              && (configuration_.count() == 1)
              && (configuration_.contains("const"))
              && (other.configuration_.contains("const"));
        if(!res){
            return false;
        }

        double const1 = configuration_.value("const").toDouble(&ok);
        if(!ok){
            return false;
        }
        double const2 = other.configuration_.value("const").toDouble(&ok);
        if(!ok){
            return false;
        }
        if(const1 != const2){
            return false;
        }

    }else{

        if( (configuration_.count() != other.configuration_.count())
              || (configuration_.count() != 4)){
            return false;
        }

        QStringList params;
        params << "number" << "start" << "end" << "step";
        foreach(QString param, params){
            if( !other.configuration_.contains(param)
                || !configuration_.contains(param) ){
                return false;
            }
        }

        int numberSource1, numberSource2;
        numberSource1 = configuration_.value("number",0).toInt(&ok);
        if(!ok){
            return false;
        }
        numberSource2 = other.configuration_.value("number",0).toInt(&ok);
        if(!ok){
            return false;
        }

        if( numberSource1 != numberSource1
              || numberSource1 == 0
              || numberSource2 == 0 ) {
            return false;
        }

        params.removeAt( params.indexOf("number") );

        foreach(QString param, params){

            double param1 = configuration_.value(param).toDouble(&ok);
            if(!ok){
                return false;
            }

            double param2 = other.configuration_.value(param).toDouble(&ok);
            if(!ok){
                return false;
            }

            if( param1 != param2 ){
                return false;
            }


        }
    }

    return true;
}

bool Source::compare( const QList<Source>& sources1,const QList<Source>& sources2 ) {

    if(sources1.count() != sources2.count()){
        return false;
    }

    foreach(Source source1,sources1){
        bool found=false;
        foreach(Source source2,sources2){
            if(source1 == source2){
                found = true;
                break;
            }
        }
        if(!found){
            return false;
        }
    }
    return true;
}


SourceManager::SourceManager(const QList<Source *> &sources)
    : sources_(sources)
{

}

QList<Source *> SourceManager::inputs() {
    QList<Source*> sources;
    foreach(Source* source,sources_){
        if(source->direction() == SOURCE_DIRECTION_INPUT){
            sources.append(source);
        }
    }

    return sources;
}

QList<Source *> SourceManager::outputs() {
    QList<Source*> sources;
    foreach(Source* source,sources_){
        if(source->direction() == SOURCE_DIRECTION_OUTPUT){
            sources.append(source);
        }
    }

    return sources;
}

Source *SourceManager::inputByNode(const QString &node) {
    foreach(Source* source, sources_){
        if(source->direction() == SOURCE_DIRECTION_INPUT
                && source->node().compare(node,Qt::CaseInsensitive)==0){
            return source;
        }
    }

    return NULL;
}

} // tsunami
