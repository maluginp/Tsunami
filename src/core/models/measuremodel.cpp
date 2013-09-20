#include "measuremodel.h"
#include "../components/json.h"

MeasureModel::MeasureModel()
    : measureId_(-1),projectId_(-1),enable_(false),userId_(-1),Model() {
}

const MeasureHeaderData &MeasureModel::headerData(const int &index) const {
    if( index < 0 || index >= headerData_.size() ){
        return MeasureHeaderData();
    }

    return headerData_[index];
}


void MeasureModel::parseJsonHeader(const QString &header) {

    QVariantMap jsonHeader = QtJson::parse( header ).toMap();

    QVariant value;

    foreach(QString key, jsonHeader.keys()){
        value = jsonHeader.value( key );
        if( compare( key, "type" ) ){
            if( compare( value.toString(), "dc" ) ){
                header_.type = TYPE_DC;
            }else if(compare( value.toString(), "ac" )){
                header_.type = TYPE_AC;
            }else if(compare( value.toString(), "tran" )){
                header_.type = TYPE_TRAN;
            }
        }else if(compare(key,"comment")){
            header_.comment = value.toString();
        }else if(compare(key,"user_date")){
            header_.userDate = value.toDate();
        }else if(compare(key,"fabric_date")){
            header_.fabricationDate = value.toDate();
        }else if(compare(key,"dubious")){
            header_.dubious = value.toBool();
        }else if(compare(key,"attributes")){
            header_.attributes = value.toMap();
        }
    }


}

void MeasureModel::parseJsonHeaderData(const QString &headerData) {
    headerData_.clear();
    QVariantList dataHeaders = QtJson::parse( headerData ).toList();
    QVariant value;

    foreach(QVariant dataHeader, dataHeaders) {
        QVariantMap dataHeaderItem = dataHeader.toMap();
        MeasureHeaderData header;
        foreach( QString key, dataHeaderItem.keys() ){
            value = dataHeaderItem.value( key );
            if(compare(key,"source")){
                header.source = value.toString();
            }else if(compare(key,"type")){
                if(compare(value.toString(),"voltage")){
                    header.type = SOURCE_VOLTAGE;
                }else if(compare(value.toString(),"current")){
                    header.type = SOURCE_CURRENT;
                }else if(compare(value.toString(),"gnd")){
                    header.type = SOURCE_GND;
                }
            }else if(compare(key,"method")){
                header.method = value.toString();
            }
        }
        headerData_.append( header );
    }
}

void MeasureModel::parseJsonData(const QString &jsonData) {
    bool ok;

    QVariantMap data = QtJson::parse(jsonData).toMap();

    foreach(QString key, data.keys()){
        if(compare(key,"columns")){
            data_.columns = data.value(key).toList();
        } else if(compare(key,"items")) {
            QVariantList jsonRows = data.value(key).toList();
            foreach(QVariant row, jsonRows){
                QVariantList jsonColumns = row.toList();
                QVector<double> rowItems;
                foreach(QVariant column, jsonColumns ){
                    double value = column.toDouble( &ok );
                    if(!ok){ value = 0.0; }
                    rowItems.append( value );
                }
                data_.items.append( rowItems );
            }
        }
    }
}

QString MeasureModel::jsonHeader() const {
    QVariantMap header;
    switch(header_.type){
    case TYPE_DC: header.insert("type",   QVariant("dc"));   break;
    case TYPE_AC: header.insert("type",   QVariant("ac"));    break;
    case TYPE_TRAN: header.insert("type", QVariant("tran")); break;
    }
    header.insert("comment",     QVariant(header_.comment));
    header.insert("user_date",   QVariant(header_.userDate) );
    header.insert("fabric_date", QVariant(header_.fabricationDate));
    header.insert("dubious",     QVariant(header_.dubious));
    header.insert("attributes",  header_.attributes);

    QString json("");
    json = QtJson::serializeStr(header);
    return json;
}

QString MeasureModel::jsonHeaderData() const {
    QVariantList headers;

    foreach( MeasureHeaderData headerData, headerData_ ){
        QVariantMap header;
        header.insert( "source", headerData.source );
        switch(headerData.type){
        case SOURCE_VOLTAGE: header.insert( "type", QVariant("voltage") ); break;
        case SOURCE_CURRENT: header.insert( "type", QVariant("current")); break;
        case SOURCE_GND:     header.insert( "type", QVariant("gnd")); break;
        }
        header.insert( "method", QVariant(headerData.method) );
        headers.append( header );
    }

    QString json("");
    json = QtJson::serializeStr(headers);
    return json;
}

QString MeasureModel::jsonData() const {
    QVariantMap data;
    data.insert("columns", data_.columns);
    int columns = 0;
    int rows = data_.items.size();
    if(rows != 0 ){ columns = data_.items.at(0).size(); }
    QVariantList items;
    for(int i=0; i < rows; ++i){
        QVariantList rowItems;
        for(int j=0; j < columns; ++j){
            rowItems.append( QVariant(data_.items.at(i).at(j)) );
        }
        items.append( rowItems );
    }
    data.insert( "items", items );

    QString json("");
    json = QtJson::serializeStr( data );

    return json;
}


int MeasureModel::dataRows() {
    return data_.items.size();
}

int MeasureModel::dataColumns() {
    return data_.columns.size();
}

const QVariant& MeasureModel::getColumnName(const int &section) const {
    if( section < data_.columns.size() ){
        return data_.columns.at( section );
    }
    return QVariant("");
}

MeasureModel &MeasureModel::setId(const int &id) {
    measureId_ = id;
    return *this;
}

MeasureModel &MeasureModel::setProjectId(const int &projectId) {
    projectId_ = projectId;
    return *this;
}

MeasureModel &MeasureModel::setHeader(const MeasureHeader &header) {
    header_ = header;
    return *this;
}

MeasureModel &MeasureModel::setHeaderData(const QList<MeasureHeaderData> &headerData) {
    headerData_ = headerData;
    return *this;
}

MeasureModel &MeasureModel::setHeaderData(const MeasureHeaderData &headerData) {
    QList<MeasureHeaderData> headers;
    headers.append( headerData );
    headerData_ = headers;

    return *this;
}

MeasureModel &MeasureModel::appendHeaderData(const MeasureHeaderData &headerData) {
    headerData_.append( headerData );
    return *this;
}

MeasureModel &MeasureModel::setMeasureData(const MeasureData &data) {
    data_ = data;
    return *this;
}

MeasureModel &MeasureModel::setEnable(const bool &enable) {
    enable_ = enable;
    return *this;
}

MeasureModel &MeasureModel::setUserId(const int &userId) {
    userId_ = userId;
    return *this;
}

MeasureModel &MeasureModel::setCreateAt(const QDateTime &createAt) {
    createAt_ = createAt;
    return *this;
}

MeasureModel &MeasureModel::setChangeAt(const QDateTime &changeAt) {
    changeAt_ = changeAt;
    return *this;
}

double &MeasureModel::item(const int &row, const int &column) {
    return data_.items[row][column];
}

const QVariant &MeasureModel::itemAt(const QModelIndex &index) const{
    QVariant value = data_.items[index.row()][index.column()];
    return value;
}
