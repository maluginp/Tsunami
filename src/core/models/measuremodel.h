#ifndef MEASUREMODEL_H
#define MEASUREMODEL_H

#include <QtCore>
#include <model.h>

class MeasureStorage;

typedef QVector< QVector<double> > QMatrixDouble;

enum MeasureType{TYPE_DC,TYPE_AC,TYPE_TRAN,TYPE_UNKNOWN};
enum MeasureSource{SOURCE_VOLTAGE,SOURCE_CURRENT,SOURCE_GND,SOURCE_UNKNOWN};

struct MeasureHeader{
    MeasureType type;
    QString comment;
    QDate userDate;
    QDate fabricationDate;
    bool dubious;
    QVariantMap attributes;
    MeasureHeader()
        : type(TYPE_UNKNOWN), comment(QString()), userDate(QDate::currentDate()),
          fabricationDate(QDate::currentDate()), dubious(false),
          attributes(QVariantMap()) {

    }

    MeasureHeader(const MeasureHeader& other)
        : type(other.type), comment(other.comment),
          userDate(other.userDate), dubious(other.dubious),
          fabricationDate(other.fabricationDate),
          attributes(other.attributes) {

    }
};

struct MeasureHeaderData{
    QString source;
    MeasureSource type;
    QString method;

    MeasureHeaderData()
        :source(QString()), type(SOURCE_UNKNOWN), method(QString()){

    }

    MeasureHeaderData(const MeasureHeaderData& other)
        : source(other.source), method(other.type), type(other.type) {

    }
};

struct MeasureData{
    QVariantList columns;
    QMatrixDouble items;

    MeasureData():
        columns(QVariantList()), items(QMatrixDouble()){

    }

    MeasureData(const MeasureData& other)
        :columns(other.columns),items(other.items) {
    }
    MeasureData& operator=(const MeasureData& other){
        columns = other.columns;
        items   = other.items;
        return *this;
    }
};

class MeasureModel : public Model {
friend class MeasureStorage;

public:
    MeasureModel();
    inline const int& id() const                              { return measureId_;  }
    inline const int& projectId() const                       { return projectId_;  }
    inline const int&  userId() const                         { return userId_;     }
    inline const MeasureHeader& header() const                { return header_;     }
    inline const QList<MeasureHeaderData>& headerData() const { return headerData_; }
    inline const MeasureData& measureData() const             { return data_;       }
    inline const bool& enable() const                         { return enable_;     }
    inline const QDateTime& createAt() const                  { return createAt_;   }
    inline const QDateTime& changeAt() const                  { return changeAt_;   }
    MeasureHeaderData headerData(const int& index) const;

    double item(const int &row, const int &column) const;


    QVariant itemAt(const QModelIndex& index) const;

    int dataRows() const;
    int dataColumns() const;

    QVariant getColumnName(const int& section) const;

    MeasureModel& setId(const int& id);
    MeasureModel& setProjectId(const int& projectId);
    MeasureModel& setHeader( const MeasureHeader& header );
    MeasureModel& setHeaderData( const QList<MeasureHeaderData>& headerData);
    MeasureModel& setHeaderData( const MeasureHeaderData& headerData);
    MeasureModel& appendHeaderData( const MeasureHeaderData& headerData );
    MeasureModel& setMeasureData( const MeasureData& measureData);
    MeasureModel& setEnable( const bool& enable );
    MeasureModel& setUserId( const int& userId );
    MeasureModel& setCreateAt( const QDateTime& createAt);
    MeasureModel& setChangeAt( const QDateTime& changeAt);
    MeasureModel& setItem(const int &row, const int &column, const double& value);

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

    QDateTime createAt_;
    QDateTime changeAt_;

    bool enable_;
    int userId_;
};

#endif // MEASUREMODEL_H
