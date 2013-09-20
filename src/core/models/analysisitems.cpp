#include "analysisitems.h"
#include "../components/json.h"

AnalysisItems::AnalysisItems()
{
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
       }
   }
}
