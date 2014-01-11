#ifndef PARSEMEASUREMODEL_H
#define PARSEMEASUREMODEL_H
#include <QString>
#include "models/MeasureModel.h"
#include "Source.h"

namespace tsunami{
class ParseMeasureModel {
public:
    ParseMeasureModel(const QByteArray& data);

    static db::MeasureModel* parse(const QByteArray& data);
private:
    QPair<QString,QVariant> readAttribute( const QString& data );
    Source readSource( const QString& data);
    QVector<double> readDataRow( const QString& data  );
    QStringList readColumns(const QString& data);
    QPair<QString,QVariant> readConfigParam(const QString& data,char split='=');
    QString readElement(const QString& elementName);
    QString readElement(const QString& data, const QString& elementName);
    QString readSection(const QString& sectionName);

    QString data_;
    db::MeasureModel* model_;


};
} // tsunami
#endif // PARSEMEASUREMODEL_H
