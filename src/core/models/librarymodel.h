#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H
#include <QDateTime>
#include <Model.h>
#include <QModelIndex>
//#include "../dbstorage/parameterstorage.h"
namespace tsunami {
namespace db{

class ParameterModel;

class LibraryModel : public Model {
//    friend class ParameterStorage;
public:
    LibraryModel();

    void id(int libraryId);
    void deviceId(int deviceId);
    void name(const QString& name);
    void createAt(const QDate& createAt);
    void changeAt(const QDate& changeAt);
    void enable(bool enable);

    const int& id() const;
    const int& deviceId() const;
    const QString& name() const;
    const QDate& createAt() const;
    const QDate& changeAt() const;
    const bool& enable() const;

    const ParameterModel& parameter(const QString& name) const;
    const ParameterModel& parameter(int index) const;

    ParameterModel& parameter(const QString& name);
    ParameterModel& parameter(int index);

    const QList<ParameterModel>& parameters() const;


    void addParameter( const ParameterModel& parameter );
    bool parameterExists( const QString& name );
    void setParameter(const QString& name, const ParameterModel& parameter);

protected:
    ParameterModel& find(const QString& name);
    ParameterModel& find(int index);

private:
    int libraryId_;
    int deviceId_;
    QString name_;
    QDate createdAt_;
    QDate changedAt_;
    bool enable_;

    QList<ParameterModel> parameters_;
};

}
}
#endif // LIBRARYMODEL_H
