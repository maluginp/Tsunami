#ifndef MEASUREMODEL_H
#define MEASUREMODEL_H

#include "defines.h"

namespace tsunami {
namespace db{

struct MeasureHeader{
    QString comment;
    QDate fabricationDate;
    QDate userDate;
    bool dubious;
    MeasureHeader() :
        fabricationDate(QDate::currentDate()),
        userDate( QDate::currentDate() ),
        dubious(false){

    }

    MeasureHeader(const QString& _comment,
                  const QDate& _fabricationDate = QDate::currentDate(),
                  const QDate& _userDate = QDate::currentDate(),
                  bool _dubious = false
                  ){
        comment = _comment;
        fabricationDate = _fabricationDate;
        userDate = _userDate;
        dubious = _dubious;
    }
    MeasureHeader(const MeasureHeader& other){
        comment = other.comment;
        fabricationDate = other.fabricationDate;
        userDate = other.userDate;
        dubious = other.dubious;
    }
    MeasureHeader& operator=(const MeasureHeader& other){
        comment = other.comment;
        fabricationDate = other.fabricationDate;
        userDate = other.userDate;
        dubious = other.dubious;
        return *this;
    }
};

class TSUNAMI_EXPORT MeasureModel {
//friend class MeasureStorage;
public:
    MeasureModel();
    MeasureModel(const MeasureModel& other);
    MeasureModel& operator=(const MeasureModel& other);
    void id( int measureId ) { measureId_ = measureId; }
    void deviceId( int deviceId ) { deviceId_ = deviceId; }
    void name( const QString& name ) { name_ = name; }
    void type( AnalysisType type ) { type_ = type; }
    void type( const QString& type);
    void analyses( const QString& json);
    void analyses(const QVariantList& analyses);
    void attrs( const QVariantMap& attributes ) { attributes_ = attributes; }
    void attrsJson( const QString& json );
    void addAttr( const QString& key, const QVariant& value )  { attributes_.insert( key,value ); }
    void addAttr( const QPair<QString,QVariant>& pair);
    void sources( const QList<Source*>& sources ) { sources_ = sources; }
    void sourcesJson( const QString& json);
    void addSource( Source* source ) { sources_.append( source ); }
    void header( const MeasureHeader& header ) { header_ = header; }
    void header( const QString& comment, const QDate& fabrication, const QDate& user,
                 bool dubious );
    void headerJson( const QString& json );

    void columns( const QStringList& columns ) { columns_ = columns; }
    void columnsJson( const QString& json);
    void addColumn( const QString& column ) { columns_.append( column ); }

    void dataJson( const QString& json );
    void data( const QVector< QVector<double> >& data);
    QVector< QVector<double> > data();
    void createAt( const QDateTime& createAt ) { createdAt_ = createAt; }
    void changeAt( const QDateTime& changeAt ) { changedAt_ = changeAt; }

    void enable(bool enable) { enable_ = enable; }
    void userId( int userId ) { userId_ = userId; }

    int rows() const { return rows_; }

    const int& id() const { return measureId_; }
    const int& deviceId() const { return deviceId_; }
    const QString& name() const { return name_; }
    const AnalysisType& type() const { return type_; }
    QString typeJson() const;
    const QVariantMap& attrs() const { return attributes_; }
    QString attrsJson() const;
    const QList<Source*>& sources() const { return sources_; }
    Source* getSource( const QString& name );
    const MeasureHeader& header() const { return header_; }
    QString headerJson();
    QString dataJson();
    QString sourcesJson();
    QString analysesJson();
    const QStringList& columns() const { return columns_; }
    QString columnsJson();
    const QDateTime& createAt() const { return createdAt_; }
    const QDateTime& changeAt() const { return changedAt_; }
    const int& userId() const { return userId_; }
    const bool& enable() const { return enable_; }

    bool hasAttr( const QString& key, const QVariant& value = QVariant() );
    QMap<QString, double> get(int row, const QStringList& onlyColumns = QStringList()) const;

    int countSource( );

    QMap<QString,double> find( const  QMap<QString,double>& data );

    int numberAnalyses();
    QList<double> analysisValues(int i);

    const QVariantList& analyses() const;

    const double &at(int row, const QString& name) const;
    const double &at(int row, int column) const;
    double &at(int row, int column);
    const QString& column( int index ) const;

    int dataRows() const;
    int dataColumns();


    bool isFixed(const QString& name);


    static MeasureModel* importFrom( const QByteArray& data );
    static QByteArray exportTo( const MeasureModel* model );

private:
    int measureId_;
    int deviceId_;
    QString name_;
    AnalysisType type_;
    QVariantMap attributes_;
    QList<Source*> sources_;
    MeasureHeader header_;
    QStringList columns_;

    QVariantList analyses_;

    MatrixDouble* data_;
    QDateTime createdAt_;
    QDateTime changedAt_;
    bool enable_;
    int userId_;

    int rows_;


};

}
}
#endif // MEASUREMODEL_H
