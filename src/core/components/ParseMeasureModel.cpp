#include "ParseMeasureModel.h"
namespace tsunami{

ParseMeasureModel::ParseMeasureModel(const QByteArray& data) {
    data_ = data;
    data_.replace("\r","");
    model_ = new db::MeasureModel();

    QString typeModel = readElement("TYPE");
    model_->type( typeModel );
    QString dataAttributes = readSection( "ATTRIBUTES" );
    QList<QString> attrs = dataAttributes.split('\n',QString::SkipEmptyParts);
    foreach(QString attr,attrs){
        model_->addAttr( readAttribute( attr ) );
    }


    QString dataSources = readSection("SOURCES");
    QList<QString> parseSources = dataSources.split('\n',QString::SkipEmptyParts);
    foreach(QString source,parseSources){
        model_->addSource( readSource(source) );
    }

    QString dataMeasure = readSection("MEASURE");
    QList<QString> measures = dataMeasure.split('\n',QString::SkipEmptyParts);
    QStringList columns = readColumns(measures[0]);


    foreach(Source source, model_->sources()){
        if(source.direction() == SOURCE_DIRECTION_INPUT){
            if(source.method() == SOURCE_METHOD_CONST
                    || source.mode() == SOURCE_MODE_GND){
                columns.append(  source.name() );
            }
        }
    }

    model_->columns( columns );

    measures.pop_front();
    QVector< QVector<double> > items;
    foreach(QString measure, measures){
        QVector<double> dataRow = readDataRow(measure);
        // Добавить из источников
        foreach(Source source, model_->sources()){
            if(source.direction() == SOURCE_DIRECTION_INPUT){
                if(source.method() == SOURCE_METHOD_CONST
                   && source.mode() != SOURCE_MODE_GND){
                    dataRow.append( source.configuration("const").toDouble() );
                }else if(source.mode() == SOURCE_MODE_GND){
                    dataRow.append(.0);
                }
            }
        }

        items.append( dataRow );
    }

    model_->data( items );
}

db::MeasureModel *ParseMeasureModel::parse(const QByteArray &data) {
    ParseMeasureModel parser(data);

    return parser.model_;
}

QPair<QString, QVariant> ParseMeasureModel::readAttribute(const QString &data) {
    QPair<QString,QVariant> pair;

    QString dataAttribute = readElement(data,"ATTRIBUTE");

    QStringList attr = dataAttribute.split(' ',QString::SkipEmptyParts);
    if(attr.count() == 2){
        pair.first = attr.value(0);
        pair.second = QVariant(attr.value(1));
    }

    return pair;
}

Source ParseMeasureModel::readSource(const QString &data) {
//   SOURCE INPUT C VOLTAGE LINEAR end=10 number=2 start=0 step=5
    QString sourceElement = readElement(data,"SOURCE");
    Source source;

    QStringList elementAttrs = sourceElement.split(" ",QString::SkipEmptyParts);

    if(elementAttrs.count() >= 3){
        source.direction( elementAttrs[0] );
        source.node( elementAttrs[1] );
        source.mode( elementAttrs[2] );
        if(source.mode() != SOURCE_MODE_GND
           && source.direction() != SOURCE_DIRECTION_OUTPUT){

            source.method( elementAttrs[3] );
            int countElements = elementAttrs.count();
            for(int i=4; i < countElements; ++i){
                source.addConfiguration( readConfigParam(elementAttrs[i]) );
            }
        }
    }

    return source;
}

QVector<double> ParseMeasureModel::readDataRow(const QString &data) {
    QList<QString> rowData = data.split(',');
    QVector<double> row;
    bool ok;
    foreach(QString itemData, rowData){
        double value = itemData.toDouble(&ok);

        row << value;
    }
    return row;
}

QStringList ParseMeasureModel::readColumns(const QString &data) {
    QStringList columns;
    QString dataColumns = data.trimmed();
    if(dataColumns.startsWith("#")){
        dataColumns.remove(0,1);
        columns = dataColumns.split(",",QString::SkipEmptyParts);
    }
    return columns;
}

QPair<QString, QVariant> ParseMeasureModel::readConfigParam(const QString &data, char split) {
    QPair<QString, QVariant> pair;

    QStringList param = data.split(split,QString::SkipEmptyParts);
    if(param.count() == 2){
        pair.first = param[0];
        pair.second = QVariant(param[1]);
    }

    return pair;

}

QString ParseMeasureModel::readElement(const QString &elementName) {
    return readElement(data_,elementName);
}

QString ParseMeasureModel::readElement(const QString &data, const QString &elementName) {
    QString element = QString("%1 ").arg(elementName).toUpper();

    int elementStartPos = data.indexOf(element)+element.count();
    int elementEndPos   = data.indexOf("\n",elementStartPos);
    QString dataElement = data.mid( elementStartPos,
                                       elementEndPos - elementStartPos );

    return dataElement;
}

QString ParseMeasureModel::readSection(const QString &sectionName) {
    QString beginSection = QString("BEGIN_%1").arg(sectionName).toUpper();
    QString endSection = QString("END_%1").arg(sectionName).toUpper();

    int beginSectionPos = data_.indexOf(beginSection)+beginSection.count();
    int endSectionPos   = data_.indexOf(endSection);

    QString data = data_.mid( beginSectionPos, endSectionPos - beginSectionPos );

    return data;
}

}
