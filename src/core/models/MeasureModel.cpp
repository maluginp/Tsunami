#include "MeasureModel.h"
#include "components/Json.h"
#include "math/Matrix.h"
#include "components/ParseMeasureModel.h"
#include "Log.h"
namespace tsunami{
namespace db{

MeasureModel::MeasureModel() :
    measureId_(-1),
    deviceId_(-1),
    createdAt_(QDateTime::currentDateTime()),
    changedAt_(QDateTime::currentDateTime()),
    data_(NULL),
    type_(ANALYSIS_UNKNOWN),
    enable_(true),
    userId_(-1)
    {

}
// @fixme Copy @var data pointer and @var rows
MeasureModel::MeasureModel(const MeasureModel& other) :
    measureId_(other.measureId_),
    deviceId_(other.deviceId_),
    name_(other.name_),
    type_(other.type_),
    attributes_(other.attributes_),
    sources_(other.sources_),
    header_(other.header_),
    columns_(other.columns_),
    createdAt_(other.createdAt_),
    changedAt_(other.changedAt_),
    enable_(other.enable_),
    userId_(other.userId_) {

}
// @fixme description upper
MeasureModel &MeasureModel::operator=(const MeasureModel &other) {

    measureId_  = other.measureId_;
    deviceId_   = other.deviceId_;
    name_       = other.name_;
    type_       = other.type_;
    attributes_ = other.attributes_;
    sources_    = other.sources_;
    header_     = other.header_;
    columns_    = other.columns_;
    createdAt_  = other.createdAt_;
    changedAt_  = other.changedAt_;
    enable_     = other.enable_;
    userId_     = other.userId_;

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

void MeasureModel::addAttr(const QPair<QString, QVariant> &pair) {
    addAttr( pair.first, pair.second );
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

QVector<QVector<double> > MeasureModel::data() {
    QVector< QVector<double> > data;
    for(int i=0; i < dataRows(); ++i) {
        QVector<double> row;
        for(int j=0; j < dataColumns(); ++j){
            row.append( data_->at(i,j) );
        }
        data.append(row);
    }
    return data;
}

QString MeasureModel::typeJson() const {
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
// @fixme need implementation method
Source MeasureModel::getSource(const QString &name) {
    QString modeChar = name[0].toLower();
//    SourceMode mode = SOURCE_MODE_
//    if(modeChar == "v"){

//    }
    return Source();
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
            name = source.title("V%NODE");
        }else if(source.mode() == SOURCE_MODE_CURRENT){
            name = source.title("I%NODE");
        }

        if(!name.isEmpty()){
            columnSearch.append( name );
        }

    }

    bool found = false;
    for( int i=0; i < rows_; ++i ){
        found = true;
        foreach(QString column, columnSearch){
            if(data.contains(column) && fabs(data[column]-at(i,column)) > 1e-15) {
                found = false;
            }else{
//                log::log << "Not found by column" << column;
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
    if(col == -1){
        return TSUNAMI_DOUBLE_MAX;
    }
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

int MeasureModel::dataRows() const {
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

MeasureModel *MeasureModel::importFrom(const QByteArray &data) {
    MeasureModel* model = ParseMeasureModel::parse( data );

    return model;
}

QByteArray MeasureModel::exportTo(const MeasureModel *model) {
    QByteArray data;

    data.append(QString("TYPE %1\n").arg(model->typeJson()));
    data.append("\n");
    // Attributes
    data.append(QString("BEGIN_ATTRIBUTES\n"));
    QVariantMap attrs = model->attrs();
    foreach( QString name, attrs.keys() ) {
        QString value = attrs.value(name,QString()).toString();
        if(!value.isEmpty()){
            data.append(QString(" ATTRIBUTE %1 %2\n")
                        .arg(name).arg(value)
                        );
        }
    }
    data.append(QString("END_ATTRIBUTES\n"));
    data.append("\n");
    // Sources
    data.append(QString("BEGIN_SOURCES\n"));
    QList<Source> sources = model->sources();
    foreach(Source source, sources){
        QString sourceData = source.title(" SOURCE %DIR %NODE %MODE %METHOD");
        if( source.configurations().count() > 0 ){
            foreach(QString name, source.configurations().keys()){
                sourceData.append(QString(" %1=%2")
                                  .arg(name)
                                  .arg(source.configuration(name).toString())
                                  );
            }
        }
        sourceData.append("\n");
        data.append(sourceData);
    }
    data.append(QString("END_SOURCES\n"));
    data.append("\n");

    data.append(QString("BEGIN_MEASURE\n"));
    QStringList columns = model->columns();
    data.append("#").append(columns.join(",").append("\n"));
    int nItems = model->dataRows();
    for(int i=0; i < nItems; ++i){
        QStringList dataRow;
        QMap<QString, double> row = model->get(i);
        foreach(QString column, columns){
            dataRow << QString::number(row.value(column,0.0));
        }
        data.append(dataRow.join(",").append("\n"));
    }
    data.append(QString("END_MEASURE\n"));

    return data;
}

} // db
} // tsunami
