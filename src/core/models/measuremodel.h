#ifndef MEASUREMODEL_H
#define MEASUREMODEL_H

#include <QtCore>
#include <model.h>

class MeasureStorage;

typedef QVector< QVector<double> > QMatrixDouble;

enum MeasureType{TYPE_DC,TYPE_AC,TYPE_TRAN};
enum MeasureSource{SOURCE_VOLTAGE,SOURCE_CURRENT,SOURCE_GND};

struct MeasureHeader{
    MeasureType type;
    QString comment;
    QDate userDate;
    QDate fabricationDate;
    bool dubious;
    QVariantMap attributes;
};

struct MeasureHeaderData{
    QString source;
    MeasureSource type;
    QString method;
};

struct MeasureData{
    QVariantList columns;
    QMatrixDouble items;

};

class MeasureModel : public Model {
friend class MeasureStorage;

public:
    MeasureModel();
    unsigned id() const;
    unsigned projectId() const;
    MeasureHeader header() const;

    int dataRows();
    int dataColumns();
    QVariant getColumnName(const int& section) const;
    QVariant item(const int& row, const int& column) const;
    QVariant& item(const int &row, const int &column);

private:
    void parseJsonHeader(const QString& header);
    void parseJsonHeaderData(const QString& headerData);
    void parseJsonData( const QString& data );

    QString jsonHeader() const;
    QString jsonHeaderData() const;
    QString jsonData() const;

    int id_;
    int projectId_;
    MeasureHeader header_;
    QList<MeasureHeaderData> headerData_;
    MeasureData data_;
    bool enable_;
    int userId_;
};

#endif // MEASUREMODEL_H
