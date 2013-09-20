#include "analysisitems.h"

AnalysisItems::AnalysisItems()
{
}


void IAnalysisItem::setMode(const IAnalysisItem::ModeType &mode) {
    mode_ = mode;
}

void IAnalysisItem::setNode(const QString &node) {
    node_ = node;
}

QString IAnalysisItem::name() const {
    QString itemName("");

    switch( mode() ){
    case VOLTAGE: itemName.append("I"); break;
    case CURRENT: itemName.append("V"); break;
    default:
        return QString();
    }

    itemName.append( node() );

    return itemName;
}


void AnalysisItemConst::parseJson(const QString &json) {
    QVariantMap attrs = QtJson::parse( json ).toMap();

    foreach(QString key, attrs.keys()){
        if( key.compare( "mode" ) == 0 ){
            QString value = attrs.value( key ).toString();
            if(value.compare( "voltage" ) == 0 ){
                setMode( VOLTAGE );
            } else if(value.compare("current") == 0){
                setMode( CURRENT );
            }
        } else if( key.compare("node") == 0){
            QString value = attrs.value( key ).toString();
            setNode( value );
        } else if( key.compare("const") == 0){
            double value  = attrs.value( key ).toDouble();
            setConstant( value );
        }
    }

}

QVariantMap AnalysisItemConst::json() {
    QVariantMap attrs;
    attrs.insert( "type", "const" );
    attrs.insert( "node", node() );

    if( mode() == VOLTAGE  ){
        attrs.insert( "mode", "voltage" );
    }else if( mode() == CURRENT ){
        attrs.insert( "mode", "current" );
    }

    attrs.insert("const",constant());

    return attrs;
}

void AnalysisItemConst::setConstant(const double &constant) {
    constant_ = constant;
}


void AnalysisItemSweep::setNumber(const int &number) {
    number_ = number;
}

void AnalysisItemSweep::setMethod(const QString &method) {
    method_ = method;
}

void AnalysisItemSweep::setStart(const double &start) {
    start_ = start;
}

void AnalysisItemSweep::setStop(const double &stop) {
    stop_ = stop;
}

void AnalysisItemSweep::setStep(const double &step) {
    step_ = step;
}

QVariantMap AnalysisItemSweep::json() {
    QVariantMap attrs;
    attrs.insert( "type", "sweep" );
    attrs.insert( "node", node() );

    if( mode() == VOLTAGE  ){
        attrs.insert( "mode", "voltage" );
    }else if( mode() == CURRENT ){
        attrs.insert( "mode", "current" );
    }

    attrs.insert("number", number());
    attrs.insert("method", method() );
    attrs.insert("start",  start());
    attrs.insert("stop",   stop() );
    attrs.insert("step",   step());

    return attrs;
}

void AnalysisItemSweep::parseJson(const QString &json) {
   QVariantMap attrs = QtJson::parse( json ).toMap();
   foreach(QString key, attrs.keys()){
       if( key.compare( "mode" ) == 0 ){
           QString value = attrs.value( key ).toString();
           if(value.compare( "voltage" ) == 0 ){
               setMode( VOLTAGE );
           } else if(value.compare("current") == 0){
               setMode( CURRENT );
           }
       } else if( key.compare("node") == 0){
           QString value = attrs.value( key ).toString();
           setNode( value );
       } else if( key.compare("number") == 0){
           int value = attrs.value( key ).toInt();
           setNumber(value);
       } else if( key.compare("method") == 0){
           QString value = attrs.value( key ).toString();
           setMethod( value );
       } else if( key.compare("start") == 0){
           double value = attrs.value( key ).toDouble();
           setStart(value);
       } else if( key.compare("stop") == 0){
           double value = attrs.value( key ).toDouble();
           setStop(value);
       } else if( key.compare("step") == 0){
           double value = attrs.value( key ).toDouble();
           setStep( value );
       }
   }
}


void AnalysisItemOutput::setValue(const double &value) {
    value_ = value;
}

void AnalysisItemOutput::parseJson(const QString &json) {
    QVariantMap attrs = QtJson::parse( json ).toMap();
    foreach(QString key, attrs.keys()){
        if( key.compare( "mode" ) == 0 ){
            QString value = attrs.value( key ).toString();
            if(value.compare( "voltage" ) == 0 ){
                setMode( VOLTAGE );
            } else if(value.compare("current") == 0){
                setMode( CURRENT );
            }
        } else if( key.compare("node") == 0){
            QString value = attrs.value( key ).toString();
            setNode( value );
        }
    }

}

QVariantMap AnalysisItemOutput::json() {
    QVariantMap attrs;
    attrs.insert( "type", "output" );
    attrs.insert( "node", node() );

    if( mode() == VOLTAGE  ){
        attrs.insert( "mode", "voltage" );
    }else if( mode() == CURRENT ){
        attrs.insert( "mode", "current" );
    }

    return attrs;
}
