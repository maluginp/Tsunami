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

QMap<QString, double> MeasureModel::get(int row) {
    QMap<QString, double> data;
    foreach(QString column,columns_){
        data.insert( name, at(row,column) );
    }

    return data;
}

int MeasureModel::countSource() {
    return sources_.size();
}

QMap<QString, double> MeasureModel::find(const QMap<QString, double> &data) {
    QStringList columnSearch;

    // Getting sources;
    foreach(Source source, sources_){
        QString name;
        if(source.mode == SOURCE_MODE_VOLTAGE){
            name = QString( "V%1" ).arg(source.node);
        }else if(source.mode == SOURCE_MODE_CURRENT){
            name = QString("I%1").arg(source.node);
        }

        if(!name.isEmpty()){
            columnSearch.append( name );
        }

    }


    bool found = false;
    for( int i=0; i < rows_; ++i ){
        found = true;
        foreach(QString column, columnSearch){
            if( data.contains(column) ){
                if(data[column] != at(i,column)){
                    found = false;
                }
            }
        }
    }




}

double MeasureModel::at(int row,const QString &name ) {
    int col = columns_.indexOf(name);
    return at(row,col);
}

double MeasureModel::at(int row, int column) {
    Q_ASSERT(row >= 0 && row < rows_);
    int columns = columns_.size();
    return data_[columns*row+column];
}


}
}
