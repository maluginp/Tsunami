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
    void createAt(const QDateTime& createAt);
    void changeAt(const QDateTime& changeAt);
    void enable(bool enable);

    const int& id() const;
    const int& deviceId() const;
    const QString& name() const;
    const QDateTime& createAt() const;
    const QDateTime& changeAt() const;
    const bool& enable() const;

    const ParameterModel& parameter(const QString& name) const;
    const ParameterModel& parameter(int index) const;

    ParameterModel& parameter(const QString& name);
    ParameterModel& parameter(int index);

    const QList<ParameterModel>& parameters() const;


    void addParameter( const ParameterModel& parameter );
    bool parameterExists( const QString& name );
    void setParameter(const QString& name, const ParameterModel& parameter);

    int countParamaters();

protected:
    ParameterModel& find(const QString& name);
    ParameterModel& find(int index);

private:
    int libraryId_;
    int deviceId_;
    QString name_;
    QDateTime createdAt_;
    QDateTime changedAt_;
    bool enable_;

    QList<ParameterModel> parameters_;
};

}
}
#endif // LIBRARYMODEL_H
