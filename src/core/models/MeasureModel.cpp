#include "MeasureModel.h"
#include "components/Json.h"
#include "math/Matrix.h"
#include <QtXml>
#include "Log.h"
namespace tsunami{
namespace db{

MeasureModel::MeasureModel() : data_(NULL){

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

    log::logDebug() << "Import measure model from " << data.count() << " bytes";

    MeasureModel* model = new MeasureModel();
    QXmlStreamReader* reader = new QXmlStreamReader(data);

    while( !reader->atEnd() ){
        QXmlStreamReader::TokenType token = reader->readNext();
        if(token == QXmlStreamReader::StartElement){
            if( reader->name() == "measure" ){
                QXmlStreamAttributes attrs = reader->attributes();
                model->type( attrs.value("type").toString() );
//                model->createAt( attrs.value("createAt").toString() );
            }else if( reader->name() == "attributes" ){
                while( true ){
                    token = reader->readNext();
                    if(reader->name() == "attribute" && token == QXmlStreamReader::StartElement){
                        QXmlStreamAttributes attrs = reader->attributes();
                        model->addAttr( attrs.value("name").toString(), attrs.value("value").toString() );
                    }else if( reader->name() == "attributes" && token == QXmlStreamReader::EndElement ){
                        break;
                    }
                }
            }else if( reader->name() == "sources"){
                while( true ){
                    token = reader->readNext();
                    if(reader->name() == "source" && token == QXmlStreamReader::StartElement){
                        QXmlStreamAttributes attrs = reader->attributes();
                        Source source;
                        source.node(  attrs.value("node").toString() );
                        source.mode( attrs.value("mode").toString() );
                        source.direction( attrs.value("direction").toString() );
                        source.method( attrs.value("method").toString() );

                        if( source.method() == SOURCE_METHOD_CONST){
                            source.addConfiguration( "const", attrs.value("const").toString() );
                        }else if( source.method() == SOURCE_METHOD_LINEAR){
                            source.addConfiguration( "start", attrs.value("start").toString() );
                            source.addConfiguration( "step", attrs.value("step").toString() );
                            source.addConfiguration( "end", attrs.value("end").toString() );
                            source.addConfiguration( "number", attrs.value("number").toString() );
                        }

                        model->addSource( source );
                    }else if( reader->name() == "sources" && token == QXmlStreamReader::EndElement ){
                        break;
                    }
                }
            }else if( reader->name() == "columns" ){
                while( true ){
                    token = reader->readNext();
                    if(reader->name() == "column" && token == QXmlStreamReader::StartElement){
                        QXmlStreamAttributes attrs = reader->attributes();
                        model->addColumn( attrs.value("name").toString() );
                    }else if(reader->name() =="columns" && token == QXmlStreamReader::EndElement){
                        break;
                    }
                }
            }else if( reader->name() == "items"){
                QStringList columns = model->columns();
                QVector< QVector<double> > data;
                while( true ){
                    token = reader->readNext();
                    if(reader->name() == "item" && token == QXmlStreamReader::StartElement){
                        QVector<double> row;
                        QXmlStreamAttributes attrs = reader->attributes();

                        foreach(QString column, columns){
                            row.append( attrs.value(column).toString().toDouble() );
                        }

                        data.append( row );
                    }else if(reader->name() == "items" && token == QXmlStreamReader::EndElement){
                        break;
                    }

                }
                model->data( data );
            }
        }
    }

    delete reader;

    log::logDebug() << "Imported measure: " << model->typeJson()
                    << "with" << model->dataRows()
                    << "items (" << model->dataColumns() << "columns)";

    return model;
}

QByteArray MeasureModel::exportTo(const MeasureModel *model) {
    QByteArray data;

    QXmlStreamWriter* writer = new QXmlStreamWriter(&data);
    writer->setAutoFormatting(true);
    writer->writeStartDocument();
    writer->writeStartElement("measure");
//    writer->writeAttribute("name",model->name());
    writer->writeAttribute("type",model->typeJson());
//    writer->writeAttribute("created",model->createAt().toString());
//    writer->writeAttribute("changed",model->changeAt().toString());
//    writer->writeAttribute("enable",);
    writer->writeStartElement("attributes");
    QVariantMap attrs = model->attrs();
    foreach( QString name, attrs.keys() ){
        writer->writeStartElement("attribute");
        writer->writeAttribute("name",name);
        writer->writeAttribute("value",attrs.value(name,QVariant()).toString());
        writer->writeEndElement();
    }
    writer->writeEndElement();

    writer->writeStartElement("sources");
    QList<Source> sources = model->sources();
    foreach(Source source, sources){
        writer->writeStartElement("source");
        writer->writeAttribute("node",source.node());
        writer->writeAttribute("mode",source.modeJson());
        writer->writeAttribute("direction",source.directionJson());
        writer->writeAttribute("method",source.methodJson());

        if( source.configurations().count() > 0 ){
            foreach(QString name, source.configurations().keys()){
                writer->writeAttribute(name, source.configuration(name).toString());
            }
        }

        writer->writeEndElement();
    }
    writer->writeEndElement();

    writer->writeStartElement("columns");
    foreach(QString column, model->columns()){
        writer->writeStartElement("column");
        writer->writeAttribute("name", column);
        writer->writeEndElement();
    }
    writer->writeEndElement();

    writer->writeStartElement("items");
    int nItems = model->dataRows();
    for(int i=0; i < nItems; ++i){
         QMap<QString, double> row = model->get(i);
         writer->writeStartElement("item");
         foreach(QString name,row.keys()){
             writer->writeAttribute( name, QString::number( row.value(name,.0)) );
         }
         writer->writeEndElement();
    }
    writer->writeEndElement();

    writer->writeEndElement();
    writer->writeEndDocument();

    return data;
}

} // db
} // tsunami
