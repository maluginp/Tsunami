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
    inline const int& id() const                              { return measureId_;  }
    inline const int& projectId() const                       { return projectId_;  }
    inline const MeasureHeader& header() const                { return header_;     }
    inline const QList<MeasureHeaderData>& headerData() const { return headerData_; }
    inline const MeasureData& data() const                    { return data_;       }
    inline const bool& enable() const                         { return enable_;     }
    inline const int&  userId() const                         { return userId_;     }
    const MeasureHeaderData& headerData(const int& index) const;


    double &item(const int &row, const int &column);
    const QVariant& itemAt(const int& row, const int& column) const;

    int dataRows();
    int dataColumns();

    const QVariant& getColumnName(const int& section) const;

    MeasureModel& setId(const int& id);
    MeasureModel& setProjectId(const int& projectId);
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

    int measureId_;
    int projectId_;
    MeasureHeader header_;
    QList<MeasureHeaderData> headerData_;
    MeasureData data_;
    bool enable_;
    int userId_;
};

#endif // MEASUREMODEL_H
