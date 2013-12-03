#include "measuremodel.h"
#include "../components/json.h"
#include "math/matrix.h"

namespace tsunami{
namespace db{

MeasureModel::MeasureModel() : data_(NULL){

}
MeasureModel::MeasureModel(const MeasureModel& other){
    Q_ASSERT(false);
}

MeasureModel &MeasureModel::operator=(const MeasureModel &other)
{
    Q_ASSERT(false);
    return *this;
}



void MeasureModel::type( const QString& type){

    if(type.compare("dc",Qt::CaseInsensitive) == 0){
        type_ = ANALYSIS_DC;
    }else if(type.compare("ac",Qt::CaseInsensitive) == 0){
        type_ = ANALYSIS_AC;
    }else if(type.compare("tran",Qt::CaseInsensitive) == 0){
        type_ = ANALYSIS_TRAN;
    }else{
        Q_ASSERT(false);
        type_ = ANALYSIS_UNKNOWN;
    }



}

void MeasureModel::header(const QString &comment, const QDate &fabrication, const QDate &user, bool dubious) {
    MeasureHeader header;
    header.comment = comment;
    header.fabricationDate = fabrication;
    header.userDate = user;
    header.dubious = dubious;
    header_ = header;
}

void MeasureModel::attrsJson( const QString& json ){
    QVariantMap attrs = QtJson::parse(json).toMap();
    attributes_ = attrs;
}

void MeasureModel::sourcesJson(const QString &json) {
    sources_.clear();
    QVariantList sources = QtJson::parse( json ).toList();

    for(int i=0; i < sources.size(); ++i) {
        QVariantMap sourceJson = sources[i].toMap();
        Source source;
        source.node(sourceJson.value( "node" ).toString());
        source.method(sourceJson.value("method").toString());
        source.configurations(sourceJson.value("configuration",QVariantMap()).toMap());
        source.mode( sourceJson.value("mode").toString() );
        source.direction(sourceJson.value("direction").toString());

        sources_.append(source);

    }
}

void MeasureModel::headerJson( const QString& json ){
    QVariantMap header = QtJson::parse(json).toMap();

    header_ = MeasureHeader( header.value("comment").toString(),
                             header.value("fabrication_date").toDate(),
                             header.value("user_date").toDate(),
                             header.value("dubious").toBool());

}


void MeasureModel::columnsJson(const QString &json) {
    QStringList list = QtJson::parse(json).toStringList();
    columns_ = list;
}

void MeasureModel::dataJson(const QString &json) {
    QVariantMap dataJson = QtJson::parse(json).toMap();
    rows_ = dataJson.size();
    int columns = dataJson.value("0").toList().size();

    delete data_;
    data_ = new MatrixDouble(rows_,columns,MatrixDouble::MATRIX_ZERO);
    for(int i=0; i < rows_; ++i){
        QVariantList rowJson = dataJson[ QString::number(i) ].toList();
        for(int j=0; j < columns; ++j){
            data_->at(i,j) = rowJson[j].toDouble();
        }
    }


}

void MeasureModel::data(const QVector<QVector<double> > &data) {

    Q_ASSERT(data.size() > 0);

    rows_ = data.size();
    int columns = data[0].size();

    data_ = new MatrixDouble(rows_,columns,MatrixDouble::MATRIX_ZERO);
    for(int i=0; i < rows_; ++i){
        for(int j=0; j < columns; ++j){
            data_->at(i,j) = data.at(i).at(j);
        }
    }


}

QString MeasureModel::typeJson() {
    switch(type_){
    case ANALYSIS_AC: return QString("ac");
    case ANALYSIS_DC: return QString("dc");
    case ANALYSIS_TRAN: return QString("tran");
    }
    Q_ASSERT(false);
    return QString();
}

QString MeasureModel::attrsJson() const {
    return QtJson::serializeStr(attributes_);
}

Source MeasureModel::getSource(const QString &name) {
    QString modeChar = name[0].toLower();
//    SourceMode mode = SOURCE_MODE_
//    if(modeChar == "v"){

//    }
}

QString MeasureModel::headerJson() {
    QString json;

    QVariantMap header;
    header.insert( "comment", header_.comment );
    header.insert("fabrication_date", header_.fabricationDate);
    header.insert("user_date", header_.userDate);
    header.insert("dubious",header_.dubious);

    json = QtJson::serializeStr(header);

    return json;
}

QString MeasureModel::dataJson() {
    int rows = data_->rows();
    int columns = data_->columns();
    QVariantMap dataJson;
    for(int i=0; i < rows; ++i){
        QVariantList rowJson;
        for(int j=0; j < columns; ++j){
            rowJson.append(data_->at(i,j));
        }
        dataJson.insert( QString::number(i), rowJson );
    }

    QString json = QtJson::serializeStr(dataJson);
    return json;
}

QString MeasureModel::sourcesJson() {
    QString json;

    QVariantList sources;

    foreach(Source source, sources_){
        QVariantMap sourceJson;
        sourceJson.insert( "node",   source.node() );
        sourceJson.insert( "method", source.methodJson());
        sourceJson.insert( "configuration", source.configurations() );
        sourceJson.insert( "mode",   source.modeJson());

        if(source.direction() == SOURCE_DIRECTION_INPUT){
            sourceJson.insert("direction","input");
        }else if(source.direction() == SOURCE_DIRECTION_OUTPUT){
            sourceJson.insert("direction","output");
        }else{
            Q_ASSERT(false);
        }


        sources.append( sourceJson );

    }

    json = QtJson::serializeStr( sources );

    return json;

}
 QString MeasureModel::columnsJson() {
    QVariantList items;
    foreach(QString item,columns_){
        items.append( item );
    }

    QString json = QtJson::serializeStr( items );
    return json;

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

QMap<QString, double> MeasureModel::get(int row) const {
    QMap<QString, double> data;
    foreach(QString column,columns_){
        data.insert( column, at(row,column) );
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
        if(source.direction() != SOURCE_DIRECTION_INPUT){
            continue;
        }

        if(source.mode() == SOURCE_MODE_VOLTAGE){
            name = QString( "V%1" ).arg(source.node().toLower());
        }else if(source.mode() == SOURCE_MODE_CURRENT){
            name = QString("I%1").arg(source.node().toLower());
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
                if(fabs(data[column] - at(i,column)) > 1e-15) {
                    found = false;
                }
            }else{
                qDebug() << "Not found by column" << column;
                found = false;
            }
        }

        if(found){
            return get(i);
        }

    }

    return QMap<QString, double>();



}

const double &MeasureModel::at(int row,const QString &name ) const {
    int col = columns_.indexOf(name);
    return data_->at(row,col);
}

const double &MeasureModel::at(int row, int column) const {
    return data_->at(row,column);
}

double &MeasureModel::at(int row, int column) {
    return data_->at(row,column);

}

const QString &MeasureModel::column(int index) const {
    return columns_[index];
}

int MeasureModel::dataRows() {
    if(data_ == 0){ return 0; }
    return data_->rows();
}

int MeasureModel::dataColumns() {
    if(data_ == 0){ return 0; }
    return data_->columns();
}

bool MeasureModel::isSourceDirection(const QString &name, SourceDirection direction) {

    foreach( Source source, sources_ ){
        if(source.name().compare(name,Qt::CaseInsensitive) == 0){
            if(source.direction() == direction){
                return true;
            }
            break;
        }

    }

    return false;

}


}
}
