#include "measuremodel.h"
#include "../components/json.h"
namespace tsunami{
namespace db{

void MeasureModel::header(const QString &comment, const QDate &fabrication, const QDate &user, bool dubious) {
    MeasureHeader header;
    header.comment = comment;
    header.fabricationDate = fabrication;
    header.userDate = user;
    header.dubious = dubious;
    header_ = header;
}


void MeasureModel::headerJson( const QString& json ){
    // TODO Header is extracted from json
}

QString MeasureModel::headerJson() {
    // TODO JSON is formed from header struct
}

bool MeasureModel::hasAttr(const QString &key, const QVariant &value) {
    if(!value.isValid()){
        return attributes_.contains(key);
    }else{
        if(attributes_.contains(key)){
            return (attributes_.value(key) == value);
        }
    }
    return false;
}

}
}
