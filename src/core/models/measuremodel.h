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
    const int& id() const;
    const int& projectId() const;
    const MeasureHeader& header() const;
    const QList<MeasureHeaderData>& headerData() const;
    const MeasureHeaderData& headerData(const int& index) const;
    const MeasureData& data() const;
    const bool& enable() const;
    const int&  userId() const;

    double &item(const int &row, const int &column);
    const QVariant& itemAt(const int& row, const int& column) const;

    int dataRows();
    int dataColumns();

    const QVariant& getColumnName(const int& section) const;


    MeasureModel& setId(const int& id);
    MeasureModel& setProjectId(const int& id);
    MeasureModel& setHeader( const MeasureHeader& header );
    MeasureModel& setHeaderData( const QList<MeasureHeaderData>& headerData);
    MeasureModel& setHeaderData( const MeasureHeaderData& headerData);
    MeasureModel& appendHeaderData( const MeasureHeaderData& headerData );
    MeasureModel& setMeasureData( const MeasureData& data);
    MeasureModel& setEnable( const bool& enable );
    MeasureModel& setUserId( const int& userId );



private:
    void parseJsonHeader(const QString& header);
    void parseJsonHeaderData(const QString& headerData);
    void parseJsonData(const QString& jsonData );

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
